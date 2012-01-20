<?php
/**
 * @file
 * Google Analytics Event Tracking Module API.
 *
 * Provides API hooks so other modules can include their own event tracking
 * arrays.
 */

/**
 * Loads the Event Tracking Selectors, Categories, Action, Label, Value, etc.
 *
 * Allows other modules to implement custom event tracking though the
 * Google Analytics Event Tracking API module.
 *
 * @ingroup google_analytics_et_api_hooks
 */
function google_analytics_et_api() {
  // Loads the user created selectors from the variable table.
  return variable_get('google_analytics_et_selectors', array());
}

