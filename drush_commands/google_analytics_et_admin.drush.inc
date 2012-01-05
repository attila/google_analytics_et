<?php
/**
 * @file
 * Google Analytics Event Tracking Module Drush Admin.
 *
 * Utilizes Drush to administrate the google analytics event tracking.
 */

/**
 * Implements some drush hook.
 */
function google_analytics_et_admin_drush_help($section) {
  switch ($section) {
    case 'drush:gaet-add-et':
      return dt("... brief help summary goes here ...");
    case 'drush:gaet-delete-et':
      return dt("... brief help summary goes here ...");
    case 'drush:gaet-override-et':
      return dt("bla bla bla");
    case 'drush:gaet-get-override-status':
      return dt("bla bla bla");
  }
}

/**
 * Implements some drush hook.
 */
function google_analytics_et_admin_drush_command() {
  $items = array();

  $items['gaet-add-et'] = array(
    'description' => "adds an event tracking rule to the list.",
    'arguments' => array(
      'selector' => 'The CSS selector to use in the event tracking rule',
      'category' => 'The intended Category that Google is to file this event under.',
      'action' => 'The action the calls this event',
      'label' => 'Optional label for the event',
      'value' => 'Optional values (Integer)',
      'noninteraction' => 'Should this event be used in calculating the bounce rate (Bool)',
    ),
    'examples' => array(
      'drush addet "p" "body" "clicked" "somebody clicked a p body" 0 FALSE' => 'Add a comedac click event.',
    ),
    'aliases' => array('addet'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $items['gaet-delete-et'] = array(
    'description' => "deletes an event tracking rule from the list.",
    'examples' => array(
      'drush delet "p"' => 'remove all event tracking with "p" as the selector.',
    ),
    'aliases' => array('delet'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $items['gaet-override-et'] = array(
    'description' => "Toggels the override event tracking option.",
    'examples' => array(
      'drush oret' => 'Toggle the override event tracking option TRUE|FALSE. (if for some reason nothing is set, this sets it to true.)',
    ),
    'aliases' => array('oret'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $items['gaet-get-override-status'] = array(
    'description' => "Checks the status of the override event tracking option.",
    'examples' => array(
      'drush getet' => 'Returns TRUE|FALSE|"not set" depending on the override eventtracking status.',
    ),
    'aliases' => array('getet'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/** 
 * 
 */
function drush_google_analytics_et_admin_gaet_add_et($selector, $category, $action, $label = '', $value = 0, $nonineration = TRUE) {
  $selector_array = array(
    'selector' => $selector,
    'category' => $category,
    'action' => $action,
    'label' => $label,
    'value' => $value,
    'noninteraction' => $nonineration,
  );

  if (_drush_google_analytics_et_admin_gaet_add_et_input_validation($selector_array)) {
    google_analytics_et_add_event_tracker($selector_array);
    foreach ($selector_array as $key => $value) {
      drush_print('   | ' . str_pad($key . ' ', 16, "-", STR_PAD_RIGHT) . ' | ' . $value);
    }
    drush_print(dt("Done!"));
  }
  else {
    drush_print(dt("An Error Occured! \nOne or more data types was incorect."));
  }
}

/** 
 * 
 */
function drush_google_analytics_et_admin_gaet_delete_et($selector) {
  google_analytics_et_remove_event_tracker($selector);

  drush_print(dt("Done!"));
}

/** 
 * Validates the user input.
 *
 * Doesn't do any xss filtering or detection, only checks to see if the input
 * types are correct.
 */
function _drush_google_analytics_et_admin_gaet_add_et_input_validation($selector_array) {
  $validattion = FALSE;

  foreach ($selector_array as $key => $value) {
    switch ($key) {
      case 'selector':
      case 'category':
      case 'action':
      case 'label':
        if (is_string($value)) {
          $validation = TRUE;
        }
        else {
          drush_set_error('NOT_STRING', dt("!key should be a string.", array('!key' => $key)));
        }

        break;
      case 'value':
        if (is_numeric($value)) {
          $validation = TRUE;
        }
        else {
          drush_set_error('NOT_NUMBER', dt("!key should be a number.", array('!key' => $key)));
        }

        break;
      case 'noninteraction':
        if (is_bool($value)) {
          $validation = TRUE;
        }
        else {
          drush_set_error('NOT_BOOLEAN', dt("!key should be a boolean.", array('!key' => $key)));
        }

        break;
    }
  }

  return $validation;
}

/**
 * Toggles the override status, TRUE|FALSE as 1|0
 */
function drush_google_analytics_et_admin_gaet_override_et() {
  $output = dt("Done!\n");

  google_analytics_et_override_event_tracking();

  $status = google_analytics_et_get_event_trackering_override_status();
  $output .= _google_analytics_et_admin_parse_override_status($status);

  return drush_print($output);
}

/**
 * Gets the override status and displays it in drush.
 */
function drush_google_analytics_et_admin_gaet_get_override_status() {
  $status = google_analytics_et_get_event_trackering_override_status();

  $output = _google_analytics_et_admin_parse_override_status($status);

  return drush_print($output);
}

/**
 * Parse the override status into a human readable sentance. For display in
 * drush.
 */
function _google_analytics_et_admin_parse_override_status($status) {
  switch ($status) {
    case GAET_STATUS_TRUE :
      $output = dt('Google Analytics Event Tracking is Being Overwritten.');
      break;
    case GAET_STATUS_FALSE :
      $output = dt('Google Analytics Event Tracking is Not Being Overritten.');
      break;
  }

  return $output;
}

