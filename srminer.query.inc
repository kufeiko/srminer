<?php

/**
 * Composes actual query string out of the special-format array.
 *
 * @param array $query
 *   All query parameters.
 *
 * @return string
 *   String prepared to be glued to endpoint URL.
 */
function srminer_build_query($query) {
  if (!isset($query['type'])) {
    $query['type'] = 'get';
  }

  $parameters = array();
  if (isset($query['parameters'])) {
    foreach ($query['parameters'] as $key => $options) {
      $parameter_function = "srminer_parameter_" . substr($key,1);
      if (function_exists($parameter_function)) {
        $parameters[] = $parameter_function($options);
      }
      else {
        watchdog('SRMiner', 'Query parameter is not implemented: %param.', array('%param' => $key));
      }
    }
  }

  $prepared = $query['entity'] . '?' . implode('&', $parameters);
  return str_replace(' ', '%20', $prepared);
}

/**
 * Runs the query and returns server response.
 *
 * @param array $query
 *   The query to execute; will be built first.
 * @param bool $build
 *   If set to true, srminer_build_query will be called on $query parameter.
 * @param string $format
 *   The format to get the output; defaults to json.
 *
 * @return object/array
 *   Result from server.
 */
function srminer_execute_query($query, $build = TRUE, $format = 'application/json') {
  if ($build) {
    $query = srminer_build_query($query);
  }

  // @todo: deal with request format.

  // Get user credentials.
  $user = variable_get('srminer_user', '');
  $pass = variable_get('srminer_pass', '');

  // Append to endpoint.
  $url = trim(variable_get('srminer_endpoint', ''), '/') . '/' . $query;

  // Fill curl parameters.
  $options['headers'] = array(
    'Accept' => $format,
    'Content-Type' => $format,
  );
  // Misc cURL options.
  $options['curl_opts'] = array(
    CURLOPT_HTTPAUTH => CURLAUTH_NTLM,
    CURLOPT_USERPWD => $user . ':' . $pass,
  );

  // Execute.
  $response = chr_curl_http_request($url, $options);

  $result = array(
    'code' => $response->code,
    'data' => drupal_json_decode($response->data),
    'error' => $response->code == 200 ? FALSE : $response->error,
  );

  // @todo: make logging optional?
  if ($result['error']) {
    watchdog('SRMiner', $result['data']['error']['message']['value'] , array() ,WATCHDOG_ERROR);
  }
  return $result;
}

/* Parameter functions */
/**
 * Builds $filter.
 *  @param array $options
 *   options and arguments for $filter.
 *  @return string
 *   Prepared $filter statement.
 */
function srminer_parameter_filter($options) {
  $fragment = '$filter=';

  foreach ($options as $operand => $fields) {
    $operand_type = 'srminer_filter_' . $operand;
    
    if (function_exists($operand_type)) {
      $fragment .= $operand_type($fields);
    }
    else {
      watchdog('SRMiner', 'Query filter is not implemented: %param.', array('%param' => $operand));
    }
  }

  return $fragment;
}

/**
 * Builds $select.
 *  @param array $options
 *   fields for $select.
 *  @return string
 *   Prepared $select statement.
 */
function srminer_parameter_select($options) {
  $fragment = '$select=' . implode(',', $options);
  return $fragment;
}

/**
 * Builds $orderby.
 *  @param array $options
 *   options and arguments for $orderby.
 *  @return string
 *   Prepared $orderby statement.
 */
function srminer_parameter_orderby($options) {
  $fragment = '$orderby=';
  foreach ($options as $type => $fields) {
    $fragment .= implode(',', $fields) . ' ' . $type;
  }
  return $fragment;
}

/**
 * Builds $top.
 *  @param int $options
 *   number of results.
 *  @return string
 *   Prepared $top statement.
 */
function srminer_parameter_top($options) {
  return '$top=' . $options;
}

/**
 * Builds $skip.
 *  @param int $options
 *   number of rows to $skip.
 *  @return string
 *   Prepared $skip statement.
 */
function srminer_parameter_skip($options) {
  return '$skip=' . $options;
}

/* filter options function */
/**
 * Builds filter condition and.
 *  @param array $options
 *   fields to operate on.
 *  @return string
 *   Prepared $filter sub-statement
 */
function srminer_filter_and($options) {
  $fragment = array();

  foreach ($options as $operand => $fields) {
    $operand_type = 'srminer_filter_' . $operand;
    
    if (function_exists($operand_type)) {
      $fragment[] = $operand_type($fields);
    }
    else {
      watchdog('SRMiner', 'Query filter is not implemented: %param.', array('%param' => $operand));
    }
  }

  return implode(' and ', $fragment);
}

/**
 * Builds filter condition or.
 *  @param array $options
 *   fields to operate on.
 *  @return string
 *   Prepared $filter sub-statement
 */
function srminer_filter_or($options) {
    $fragment = array();

  foreach ($options as $operand => $fields) {
    $operand_type = 'srminer_filter_' . $operand;
    
    if (function_exists($operand_type)) {
      $fragment[] = $operand_type($fields);
    }
    else {
      watchdog('SRMiner', 'Query filter is not implemented: %param.', array('%param' => $operand));
    }
  }

  return implode(' or ', $fragment);
}

/**
 * Builds filter condition not.
 *  @param array $options
 *   fields to operate on.
 *  @return string
 *   Prepared $filter sub-statement
 */
function srminer_filter_not($options) {
  $operand = key($options);
  $operand_type = 'srminer_filter_' . $operand;

  return 'not ' . $operand_type($options[$operand]);
}

/**
 * Builds filter condition eq.
 *  @param array $options
 *   fields to operate on.
 *  @return string
 *   Prepared $filter sub-statement
 */
function srminer_filter_eq($fields) {
  return "(" . implode(" eq ", $fields) . ")";
}

/**
 * Builds filter condition ne.
 *  @param array $options
 *   fields to operate on.
 *  @return string
 *   Prepared $filter sub-statement
 */
function srminer_filter_ne($fields) {
  return "(" . implode(" ne ", $fields) . ")";
}

/**
 * Builds filter condition startswith.
 *  @param array $options
 *   fields to operate on.
 *  @return string
 *   Prepared $filter sub-statement
 */
function srminer_filter_startswith($fields) {
  return "startswith(" . $fields[0] . "," . $fields[1] . ")";
}

/**
 * Builds filter condition substringof.
 *  @param array $options
 *   fields to operate on.
 *  @return string
 *   Prepared $filter sub-statement
 */
function srminer_filter_substringof($fields) {
  return "substringof(" . $fields[0] . "," . $fields[1] . ")";
}
