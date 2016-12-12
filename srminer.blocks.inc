<?php

/**
 * @file
 * Contains block definitions.
 */

/**
 * Implements hook_block_info().
 *
 * Defines custom blocks for srminer.
 */
function srminer_block_info() {
  $blocks['view_activity_recorded_srm'] = array(
    'info' => t('Srminer: Button activity recorded by SRM'),
    'cache' => DRUPAL_CACHE_GLOBAL,
  );
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function srminer_block_view($delta = '') {
  $blocks = array();
  switch ($delta) {
    case 'view_activity_recorded_srm':
      $blocks['subject'] = NULL;
      $blocks['content'] = _srm_activity_recorded_button();
      break;
  }
  return $blocks;
}

/**
 * Provides content for the History button block.
 */
function _srm_activity_recorded_button() {
  $user = user_uid_optional_load();

  // Displays SRM button if the user has sufficient rights.
  if (user_access('view everyones srm activity') ||  user_access('view own srm activity')) {
    $block = array(
      '#markup' => l(t('View activity recorded by SRM'), 'srminer/activity/' . $user->uid, array('classes' => array('srm-activity-recorded-button'))),
    );
    return $block;
  }
}
