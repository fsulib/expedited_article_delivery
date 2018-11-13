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
    drupal_set_message("Your configurations have been saved.");
    variable_set('expedited_article_delivery_spreadsheet_key', $form_state['values']['expedited_article_delivery_spreadsheet_key']);
    variable_set('expedited_article_delivery_allowed_roles', $form_state['values']['expedited_article_delivery_allowed_roles']);
  }
}
