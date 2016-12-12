<?php

/**
 * @file
 * Contains page content for menu callbacks.
 */

/**
 * Returns page content showing SRM activity.
 *
 * @params
 *  $uid integer ID of user whose activity will be looked up in SRM.
 *
 * @return array
 *   Renderable content array.
 */
function _srm_activity_recorded($uid) {
  $user = user_uid_optional_load();
  $url = variable_get('srminer_endpoint', FALSE) . "/ContactSet?\$select=JobTitle,SpousesName,EMailAddress1,Telephone1,ContactId,FirstName,LastName&\$filter=EMailAddress1%20eq%20'$user->mail'";

  // Displays SRM data is the user has sufficient rights.
  if (user_access('view everyones srm activity') || ($uid == $user->uid && user_access('view own srm activity'))) {
    $response = _srminer_datatest($url, variable_get('srminer_user'), variable_get('srminer_pass'));
    $data = json_decode($response->data);
    $items = array();
    foreach ($data->d->results as $result) {
      foreach ($result as $key => $value) {
        if ($key != '__metadata') {
          $items[] = array(
            'data' => $key . ': ' . $value,
          );
        }
      }
    }
    return array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#title' => NULL,
    );
  }
  else {
    drupal_access_denied();
  }
}

/**
 * Returns page content showing SRM activity.
 *
 * @params
 * $entity string Name of the SRM entity
 *
 * @return array
 *   Renderable content array.
 */
function _srminer_show_structure($entity) {
  $url = variable_get('srminer_endpoint', FALSE) . "/" . $entity;
  $response = _srminer_datatest($url, variable_get('srminer_user'), variable_get('srminer_pass'));
  $data = json_decode($response->data);
  if ($data) {
    $items = array();
    foreach ($data->d->results as $result) {
      foreach ($result as $key => $value) {
        if ($key != '__metadata') {
          $items[] = array(
            'data' => $key . ': ' . serialize($value),
          );
        }
      }
    }
    return array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#title' => NULL,
    );
  }
  else {
    return array('#markup' => 'no data');
  }
}

/**
 * Returns page content showing SRM activity.
 *
 * @return array
 *   Renderable content array.
 */
function _srminer_show_entities() {
  $url = variable_get('srminer_endpoint', FALSE);
  $response = _srminer_datatest($url, variable_get('srminer_user'), variable_get('srminer_pass'));
  $data = json_decode($response->data);
  if ($data) {
    $items = array();
    foreach ($data->d->EntitySets as $entity) {
      $items[] = array(
        'data' => l($entity, 'admin/config/services/srminer/show_structure/' . $entity),
      );
    }
  }
  return array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#title' => NULL,
  );
}

/**
 * Returns Test 2 page content.
 *
 * @return array
 *   Renderable content array.
 */
function _srminer_test2() {

  // $url = variable_get('srminer_endpoint', FALSE) . "/ConnectionSet?\$select=Name\$filter=Name";.
  $url = variable_get('srminer_endpoint', FALSE) . "/ConnectionSet?\$filter=Name%20eq%20'john%20smith'";
  // $url = variable_get('srminer_endpoint', FALSE) . "/ConnectionSet";
  //  $url = variable_get('srminer_endpoint', FALSE) . "/ConnectionSet?\$select=Name&\$filter=Name%20eq%20'MICROSOFT'";.
  $response = _srminer_datatest($url, variable_get('srminer_user'), variable_get('srminer_pass'));

  dpm($response);

  $data = json_decode($response->data);
  dpm($data);

  return array('#markup' => 'bla');

}


/**
 * Returns content for the user's SRM profile page.
 *
 * @return array
 *   Renderable content array.
 */

function _srminer_srm_profile($account) {

//  dpm($user);
//  $url = variable_get('srminer_endpoint', FALSE) . "/ContactSet?\$select=JobTitle,SpousesName,EMailAddress1,Telephone1,ContactId,FirstName,LastName&\$filter=EMailAddress1%20eq%20'$user->mail'";
//
//  // Displays SRM data is the user has sufficient rights.

  $fields = array(
    'FirstName',
    'LastName',
    'EMailAddress1',
    'EMailAddress2',
    'EMailAddress3',
    'Telephone1',
    'Telephone2',
    'Telephone3',
    'AssistantPhone',
    'MobilePhone',
    'ManagerPhone',
    'DoNotPhone',
    'new_VIP',
    'new_PRESS',
    'ModifiedOn',
    'CreatedOn',
  );

  $field_definitions = _srminer_field_definitions();

//  dpm($field_definitions);$field_definitions
//  foreach($field_keys as $field_key) {
//$srm_fields[] = $field_definitions[$field_key]['srm_key'];
//  }

//  dpm($srm_fields);

//  $fields = array(
//    'FirstName',
//    'LastName',
//    'EMailAddress1',
//    'EMailAddress2',
//    'EMailAddress3',
//    'Telephone1',
//    'Telephone2',
//    'Telephone3',
//    'AssistantPhone',
//    'MobilePhone',
//    'ManagerPhone',
//    'DoNotPhone',
//    'new_VIP',
//    'new_PRESS',
//    'ModifiedOn',
//    'CreatedOn',
//
//  );




  $query = array(
    'type' => 'get',
    'entity' => 'ContactSet',
    'parameters' => array(
      '$filter' => array(
        'or' => array(
          'eq' => array(
            'EMailAddress1', "'{$account->mail}'",
          ),
        ),
      ),
      '$select' => $fields,
//      '$top' => 5,
//      '$skip' => 15,
//      '$orderby' => array(
//        'desc' => array(
//          'field1',
//          'field2'
//        ),
//      ),
    ),
  );

  $response = srminer_execute_query($query);

//  dpm($response);
//  $fields= implode(',', $fields);
//  $url = variable_get('srminer_endpoint', FALSE) . "/ContactSet?\$filter=(EMailAddress1%20eq%20'info@example.com')&\$select=${fields}";
//  $response = _srminer_datatest($url, variable_get('srminer_user'), variable_get('srminer_pass'));
//  dpm($response);
//  dpm($response);

  $response_data = $response['data'];
  $records = $response_data['d']['results'];
  foreach($records as $record) {
    foreach ($record as $field_key => $value) {
      if ($field_key != '__metadata') {
        $srm_data[] = array(
          'label' => $field_definitions[$field_key]['pretty_name'],
          'value' => _srminer_srm_value_formatter($value, $field_definitions[$field_key]['formatter']),
        );
      }
    }
  }

//  $response = array(
//    'First name' => 'John',
//'Last name' => 'Smith',
//'Gender' => 'Male',
//'Job title' => 'Chief nonsense executive',
//'Mobile phone' => '876485263489568346',
//'Business phone' => '7863487563468',
//'Organisation name' => 'Hostile Take-over Bank',
//'Email' => 'info@example.com',
//'Email address 2' => NULL,
//'Address 1: Street 1' => 'dohdohdohdohdohdohdohdoh',
//'Address 1: Street 2' => 'dohdohdohdohdoh',
//'Address 1: Street 3' => '',
//'City' => 'doh',
//'State/Province' => 'doh',
//'ZIP/Postal Code' => 'doh',
//'Country' => 'doh',
//'Created on' => 'doh',
//'Modified on' => 'doh',
//'Source' => 'doh',
//  );

  return array(
    '#theme' => 'srm_data',
    '#srm_data' => $srm_data,
  );
}

