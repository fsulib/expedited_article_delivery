<?php

/**
 * @file
 * Administration page callbacks for the Expedited Article Delivery module.
 */

/**
 * Expedited Article Delivery administration and configuration form.
 *
 * @ingroup forms
 *
 * @return array
 *   The Drupal form definition.
 */
function expedited_article_delivery_admin_form() {

  $form = array();

  $form['expedited_article_delivery_spreadsheet_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Spreadsheet Key'),
    '#description' => t("The key for the Google Sheets spreadsheet to be used for checking allowable purchases."),
    '#default_value' => variable_get('expedited_article_delivery_spreadsheet_key', ''),
  );

  $formatted_roles = array();
  $roles = user_roles();
  foreach ($roles as $key => $value) {
    $formatted_roles[$value] = $value;
  }

  $form['expedited_article_delivery_allowed_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allowed roles'),
    '#description' => t("The roles that will be allowed to purchase articles."),
    '#default_value' => variable_get('expedited_article_delivery_allowed_roles'),
    '#options' => $formatted_roles,
  );

  $form['expedited_article_delivery_order_type_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Order Type ID'),
    '#description' => t("The numeric key for the order type to be used in Reprints API calls."),
    '#default_value' => variable_get('expedited_article_delivery_order_type_id', '4'),
  );

  $form['expedited_article_delivery_delivery_method_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Delivery Method ID'),
    '#description' => t("The numeric key for the delivery method to be used in Reprints API calls."),
    '#default_value' => variable_get('expedited_article_delivery_delivery_method_id', '5'),
  );

  $form['expedited_article_delivery_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug Mode'),
    '#description' => t("If selected, run Reprints API calls on test server."),
    '#default_value' => variable_get('expedited_article_delivery_debug', TRUE),
  );

  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reprints API Credentials'),
  );

  $form['credentials']['expedited_article_delivery_user_firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('User First Name'),
    '#description' => t("The first name of the user account making Reprints API calls."),
    '#default_value' => variable_get('expedited_article_delivery_user_firstname', ''),
  );

  $form['credentials']['expedited_article_delivery_user_lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('User Last Name'),
    '#description' => t("The last name of the user account making Reprints API calls."),
    '#default_value' => variable_get('expedited_article_delivery_user_lastname', ''),
  );

  $form['credentials']['expedited_article_delivery_user_email'] = array(
    '#type' => 'textfield',
    '#title' => t('User Email'),
    '#description' => t("The email address of the user account making Reprints API calls."),
    '#default_value' => variable_get('expedited_article_delivery_user_email', ''),
  );

  $form['credentials']['production'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reprints API Production Credentials'),
  );

  $form['credentials']['production']['expedited_article_delivery_production_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Production URL'),
    '#description' => t("The URL for the production Reprints server."),
    '#default_value' => variable_get('expedited_article_delivery_production_url', ''),
  );

  $form['credentials']['production']['expedited_article_delivery_production_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Production Username'),
    '#description' => t("The Username for the production Reprints server."),
    '#default_value' => variable_get('expedited_article_delivery_production_username', ''),
  );

  $form['credentials']['production']['expedited_article_delivery_production_password'] = array(
    '#type' => 'password',
    '#title' => t('Production Password'),
    '#description' => t("The Password for the production Reprints server."),
    '#default_value' => variable_get('expedited_article_delivery_production_password', ''),
  );

  $form['credentials']['test'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reprints API Test Credentials'),
  );

  $form['credentials']['test']['expedited_article_delivery_test_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Test URL'),
    '#description' => t("The URL for the test Reprints server."),
    '#default_value' => variable_get('expedited_article_delivery_test_url', ''),
  );

  $form['credentials']['test']['expedited_article_delivery_test_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Test Username'),
    '#description' => t("The Username for the test Reprints server."),
    '#default_value' => variable_get('expedited_article_delivery_test_username', ''),
  );

  $form['credentials']['test']['expedited_article_delivery_test_password'] = array(
    '#type' => 'password',
    '#title' => t('Test Password'),
    '#description' => t("The Password for the test Reprints server."),
    '#default_value' => variable_get('expedited_article_delivery_test_password', ''),
  );

  $form['profile_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reprints API Placeholders'),
  );

  $form['profile_defaults']['expedited_article_delivery_default_company'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Company'),
    '#description' => t("The default company to be used when making Reprints API requests."),
    '#default_value' => variable_get('expedited_article_delivery_default_company', ''),
  );

  $form['profile_defaults']['expedited_article_delivery_default_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Address'),
    '#description' => t("The default address to be used when making Reprints API requests."),
    '#default_value' => variable_get('expedited_article_delivery_default_address', ''),
  );

  $form['profile_defaults']['expedited_article_delivery_default_city'] = array(
    '#type' => 'textfield',
    '#title' => t('Default City'),
    '#description' => t("The default city to be used when making Reprints API requests."),
    '#default_value' => variable_get('expedited_article_delivery_default_city', ''),
  );

  $form['profile_defaults']['expedited_article_delivery_default_state'] = array(
    '#type' => 'textfield',
    '#title' => t('Default State'),
    '#description' => t("The default state (using a 2 letter code, for example 'FL') to be used when making Reprints API requests."),
    '#default_value' => variable_get('expedited_article_delivery_default_state', ''),
  );

  $form['profile_defaults']['expedited_article_delivery_default_country'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Country'),
    '#description' => t("The default country (using a 2 letter code, for example 'US') to be used when making Reprints API requests."),
    '#default_value' => variable_get('expedited_article_delivery_default_country', ''),
  );

  $form['profile_defaults']['expedited_article_delivery_default_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Phone Number'),
    '#description' => t("The default phone number to be used when making Reprints API requests."),
    '#default_value' => variable_get('expedited_article_delivery_default_phone', ''),
  );
  
  $form['error_messages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Error Messages'),
  );

  $form['error_messages']['expedited_article_delivery_error_login'] = array(
    '#type' => 'textarea',
    '#title' => t('Credentials Error Message'),
    '#description' => t("The message to display when user does not have correct credentials."),
    '#default_value' => variable_get('expedited_article_delivery_error_login', ''),
  );
  
  $form['error_messages']['expedited_article_delivery_error_availability'] = array(
    '#type' => 'textarea',
    '#title' => t('Article Outside Expedited Service Criteria Message'),
    '#description' => t("The message to display when the article does not meet expedited criteria."),
    '#default_value' => variable_get('expedited_article_delivery_error_availability', ''),
  );
  
  $form['error_messages']['expedited_article_delivery_error_reprints'] = array(
    '#type' => 'textarea',
    '#title' => t('Reprints Error Message'),
    '#description' => t("The message to display when reprints order returns an error."),
    '#default_value' => variable_get('expedited_article_delivery_error_reprints', ''),
  );
  
  $form['save_configuration'] = array(
    '#type' => 'submit',
    '#name' => 'saveConfiguration',
    '#default_value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for admin form for Expedited Article Delivery.
 *
 * @param array $form
 *   An array representing a form in Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function expedited_article_delivery_admin_form_submit($form, &$form_state) {
  if (empty($form_state['values']['expedited_article_delivery_spreadsheet_key'])) {
    form_set_error('expedited_article_delivery_spreadsheet_key', t('Spreadsheet key cannot be blank.'));
  }
  else {
    variable_set('expedited_article_delivery_spreadsheet_key', $form_state['values']['expedited_article_delivery_spreadsheet_key']);
    variable_set('expedited_article_delivery_allowed_roles', $form_state['values']['expedited_article_delivery_allowed_roles']);
    variable_set('expedited_article_delivery_order_type_id', $form_state['values']['expedited_article_delivery_order_type_id']);
    variable_set('expedited_article_delivery_delivery_method_id', $form_state['values']['expedited_article_delivery_delivery_method_id']);
    variable_set('expedited_article_delivery_debug', $form_state['values']['expedited_article_delivery_debug']);
    variable_set('expedited_article_delivery_user_firstname', $form_state['values']['expedited_article_delivery_user_firstname']);
    variable_set('expedited_article_delivery_user_lastname', $form_state['values']['expedited_article_delivery_user_lastname']);
    variable_set('expedited_article_delivery_user_email', $form_state['values']['expedited_article_delivery_user_email']);
    variable_set('expedited_article_delivery_production_url', $form_state['values']['expedited_article_delivery_production_url']);
    variable_set('expedited_article_delivery_production_username', $form_state['values']['expedited_article_delivery_production_username']);
    if (!empty($form_state['values']['expedited_article_delivery_production_password'])) {
      variable_set('expedited_article_delivery_production_password', $form_state['values']['expedited_article_delivery_production_password']);
    }
    variable_set('expedited_article_delivery_test_url', $form_state['values']['expedited_article_delivery_test_url']);
    variable_set('expedited_article_delivery_test_username', $form_state['values']['expedited_article_delivery_test_username']);
    if (!empty($form_state['values']['expedited_article_delivery_test_password'])) {
      variable_set('expedited_article_delivery_test_password', $form_state['values']['expedited_article_delivery_test_password']);
    }
    variable_set('expedited_article_delivery_default_company', $form_state['values']['expedited_article_delivery_default_company']);
    variable_set('expedited_article_delivery_default_address', $form_state['values']['expedited_article_delivery_default_address']);
    variable_set('expedited_article_delivery_default_city', $form_state['values']['expedited_article_delivery_default_city']);
    variable_set('expedited_article_delivery_default_state', $form_state['values']['expedited_article_delivery_default_state']);
    variable_set('expedited_article_delivery_default_country', $form_state['values']['expedited_article_delivery_default_country']);
    variable_set('expedited_article_delivery_default_phone', $form_state['values']['expedited_article_delivery_default_phone']);
    variable_set('expedited_article_delivery_error_login', $form_state['values']['expedited_article_delivery_error_login']);
    variable_set('expedited_article_delivery_error_availability', $form_state['values']['expedited_article_delivery_error_availability']);
    variable_set('expedited_article_delivery_error_reprints', $form_state['values']['expedited_article_delivery_error_reprints']);
    drupal_set_message("Your configurations have been saved.");
  }
}
