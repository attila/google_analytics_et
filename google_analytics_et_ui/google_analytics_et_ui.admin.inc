<?php
/**
 * @file
 * Provides the administration page for Google Analytics Event Tracker UI.
 */

function _google_analytics_et_ui_selector_form($selector = array(), $new = FALSE) {
  $events = array(
    'mousedown', 'submit', 'blur', 'change', 'click', 'dblclick', 'focus',
    'focusin', 'focusout', 'hover', 'keydown', 'keypress', 'keyup',
    'mouseenter', 'mouseleave', 'mouseout', 'mouseover', 'mouseup', 'select'
  );
  $events = drupal_map_assoc($events);

  $form = array(
    'selector' => array(
      '#type' => 'textfield',
      '#size' => 40,
      '#maxlength' => 128,
      '#default_value' => empty($selector['selector']) ? NULL : $selector['selector'],
    ),
    'options' => array(
      '#tree' => TRUE,
      'enabled' => array(
        '#type' => 'checkbox',
        '#default_value' => empty($selector['options']['enabled']) ? NULL : $selector['options']['enabled'],
      ),
      'event' => array(
        '#type' => 'select',
        '#options' => $events,
        '#default_value' => empty($selector['options']['event']) ? NULL : $selector['options']['event'],
      ),
      'category' => array(
        '#type' => 'textfield',
        '#default_value' => empty($selector['options']['category']) ? NULL : $selector['options']['category'],
        '#size' => 20,
        '#maxlength' => 128,
      ),
      'action' => array(
        '#type' => 'textfield',
        '#default_value' => empty($selector['options']['action']) ? NULL : $selector['options']['action'],
        '#size' => 20,
        '#maxlength' => 128,
      ),
      'label' => array(
        '#type' => 'textfield',
        '#default_value' => empty($selector['options']['label']) ? NULL : $selector['options']['label'],
        '#size' => 30,
        '#maxlength' => 128,
      ),
      'value' => array(
        '#type' => 'textfield',
        '#default_value' => empty($selector['options']['value']) ? NULL : $selector['options']['value'],
        '#size' => 10,
        '#maxlength' => 128,
      ),
      'noninteraction' => array(
        '#type' => 'checkbox',
        '#default_value' => empty($selector['options']['noninteraction']) ? 0 : $selector['options']['noninteraction'],
      ),
    ),
  );

  return $form;
}

/**
 * Administration settings page
 */
function google_analytics_et_ui_admin($form, &$form_state)  {
  $selectors = variable_get('google_analytics_et_ui_selectors', array());

  $form_state['cache'] = TRUE;
  $form['#tree'] = TRUE;

  $form['google_analytics_et_ui_selectors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Analytics Event Tracking selectors'),
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="gaetui-selectors-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['selector_count'])) {
    $form_state['selector_count'] = 1;
  }

  // Container for just the selectors.
  $form['google_analytics_et_ui_selectors']['selector'] = array(
    '#prefix' => '<div id="gaetui-selectors">',
    '#suffix' => '</div>',
    '#theme' => 'google_analytics_et_ui_selectors',
  );

  $delta = 0;
  if (!empty($selectors)) {
    $delta = count($selectors);
    // Add existing selectors
    foreach ($selectors as $key => $selector) {
      $form['google_analytics_et_ui_selectors']['selector'][] = _google_analytics_et_ui_selector_form($selector);
    }
  }
  // Add initial or additional selectors
  for ($i = 1 ; $i < $form_state['selector_count']; $i++) {
    $form['google_analytics_et_ui_selectors']['selector'][] = _google_analytics_et_ui_selector_form(array(), TRUE);
  }

  $form['google_analytics_et_ui_selectors']['add_selector'] = array(
    '#type' => 'submit',
    '#value' => t('Add selector'),
    '#description' => t('Click here to add selector.'),
    '#weight' => 1,
    '#submit' => array('google_analytics_et_ui_add_submit'),
    '#ajax' => array(
      'callback' => 'google_analytics_et_ui_selector_js_callback',
      'wrapper' => 'gaetui-selectors-fieldset-wrapper',
      'progress' => 'none'
    ),
    '#prefix' => '<div class="clearfix">',
    '#suffix' => '</div>',
  );

  if ($form_state['selector_count'] > 1) {
    $form['google_analytics_et_ui_selectors']['remove_selector'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one selector'),
      '#weight' => 2,
      '#submit' => array('google_analytics_et_ui_remove_submit'),
      '#ajax' => array(
        'callback' => 'google_analytics_et_ui_selector_js_callback',
        'wrapper' => 'gaetui-selectors-fieldset-wrapper',
        'progress' => 'none'
      ),
    );
  }

  $form = system_settings_form($form);
  // We don't want to call system_settings_form_submit(), so change #submit.
  array_pop($form['#submit']);
  $form['#submit'][] = 'google_analytics_et_ui_admin_submit';

  $form['google_analytics_et_ui_help'] = array(
    '#markup' => '
      <p>
        See <a href="http://api.jquery.com/category/selectors/">jQuery Selectors</a> for more information on the input for the "selector" field.
      </p>
      <p>
        See <a href="http://api.jquery.com/category/events/">jQuery Events</a> for more information on the input for the "event" field.
      </p>
      <p>
        There are several tokens that can be used as a string replacement with <strong>category, action, and label</strong>.
      </p>
      <dl>
        <dt>!text</dt>
        <dd>This is what would be returned from the jQuery .text() method.</dd>
        <dt>!href</dt>
        <dd>The value of the href attribute (handy with anchor tags).</dd>
        <dt>!currentPage</dt>
        <dd>The URL of the current page (this is taken with the code window.location.href</dd>
      </dl>
      <p>
        There is one more bit that is handy when configuring the module.<br />
        Passing <code>!test</code> will send an  alert to the page when the event is triggered and not send the event to Google Analytics.
      </p>
      <p>
        Event trackers <strong>could be cached</strong> depending on your Drupal settings!
      </p>
    ',
  );

  return $form;
}

function google_analytics_et_ui_add_submit($form, &$form_state) {
  $form_state['selector_count']++;
  $form_state['rebuild'] = TRUE;
}

function google_analytics_et_ui_remove_submit($form, &$form_state) {
  if ($form_state['selector_count'] > 1) {
    $form_state['selector_count']--;
  }
  $form_state['rebuild'] = TRUE;
}

function google_analytics_et_ui_selector_js_callback($form, $form_state) {
  return $form['google_analytics_et_ui_selectors'];
}

/**
 * Submit callback for Event tracker administrative settings
 *
 * Filters out empty form elements.
 */
function google_analytics_et_ui_admin_submit($form, &$form_state) {
  $selectors = $form_state['values']['google_analytics_et_ui_selectors']['selector'];

  foreach ($selectors as $key => $item) {
    if (empty($item['selector'])) {
      unset($selectors[$key]);
    }
  }

  cache_clear_all('google_analytics_et', 'cache', TRUE);

  variable_set('google_analytics_et_ui_selectors', $selectors);
  drupal_set_message(t('The configuration options have been saved.'));
}
