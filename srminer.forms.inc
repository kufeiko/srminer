<?php

/**
 * @file
 * Contains form definitions and alterations.
 */

/**
 * Callback for settings form.
 */
function _srminer_settings_form($form, &$form_state) {
  // url: http://s-cnect-crmdev1:5555/SRM/
  // domain\user:
  // pass:
  $form = array();
  $form['srminer_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('srminer_user', ''),
    '#description' => t('This may need to include domain, e.g. domain\user.'),
  );
  $form['srminer_pass'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Leave empty to keep the current password. Type a new one to update.'),
  );
  $form['srminer_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Endpoint URL'),
    '#default_value' => variable_get('srminer_endpoint', ''),
  );
  $form['srminer_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Heat!'),
  );

  return $form;
}

/**
 * Defines settings form.
 */
function _srminer_settings_form_submit($form, &$form_state) {
  variable_set('srminer_user', $form_state['values']['srminer_user']);
  if ($form_state['values']['srminer_pass']) {
    variable_set('srminer_pass', $form_state['values']['srminer_pass']);
  }
  variable_set('srminer_endpoint', $form_state['values']['srminer_endpoint']);

  drupal_set_message(t('Data saved.'));

  // Now, if endpoint is not empty, go test!
  if (variable_get('srminer_endpoint', FALSE)) {
    _srminer_datatest(variable_get('srminer_endpoint'), variable_get('srminer_user'), variable_get('srminer_pass'));
  }

}

/**
 * Defines test form.
 */
function _srminer_test_form($form, &$form_state) {
  $endpoint = variable_get('srminer_endpoint', FALSE);
  if (!$endpoint) {
    // Should I do something here??
  }
  $form = array();
  if (isset($form_state['storage'])) {
    $form['srminer_collapse'] = array(
      '#type' => 'fieldset',
      '#title' => t('Result of the query:'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['srminer_collapse']['srminer_result'] = array(
      '#markup' => $form_state['storage']['result'],
    );
  }
  $form['srminer_query'] = array(
    '#title' => t('Enter the query parameters, skipping the URL'),
    '#type' => 'textarea',
    '#description' => t('Your endpoint is <strong>@ep</strong>', array('@ep' => $endpoint)),
    '#default_value' => isset($form_state['storage']) ? $form_state['storage']['query'] : '',
  );
  $form['srminer_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Heat!'),
  );
  return $form;
}

/**
 * Prints repsonses when form is submitted.
 */
function _srminer_test_form_submit($form, &$form_state) {
  if ($url = variable_get('srminer_endpoint', FALSE)) {
    $url .= str_replace(' ', '%20', trim($form_state['values']['srminer_query']));
    $response = _srminer_datatest($url, variable_get('srminer_user'), variable_get('srminer_pass'));
  }
  else {
    $response = t("You haven't set a datapoint. Do so in Settings tab.");
  }
  if (module_exists('devel')) {
    $form_state['storage']['result'] = kprint_r(json_decode($response->data), TRUE);
  }
  else {
    $form_state['storage']['result'] = '<pre>' . print_r(json_decode($response->data), TRUE) . '</pre>';
  }
  // Rebuild the form.
  $form_state['storage']['query'] = $form_state['values']['srminer_query'];
  $form_state['rebuild'] = TRUE;
}
