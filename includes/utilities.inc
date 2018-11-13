<?php

function expedited_article_delivery_order_processor($args) {
  /*
  $gs_key = variable_get('expedited_article_delivery_spreadsheet_key', '');
  Header('Content-type: text/html');
  echo "<h1>Provided GET params:</h1>";
  echo "<ul>";
  foreach ($args as $key => $value) {
    echo "<li><strong>{$key}</strong>: {$value}</li>";
  }
  echo "</ul>";
  echo "<h1>Link to Google Sheets spreadsheet download as CSV:</h1>";
  echo "<p><a href='https://docs.google.com/spreadsheets/d/{$gs_key}/gviz/tq?tqx=out:csv'>https://docs.google.com/spreadsheets/d/{$gs_key}/gviz/tq?tqx=out:csv</a></p>";
  echo "<h1>Data contained in spreadsheet:</h1>\n";
  $gs_data = file_get_contents("https://docs.google.com/spreadsheets/d/{$gs_key}/gviz/tq?tqx=out:csv");
  print_r($gs_data);
  */

  // issn pages volume issue  
  global $user;
  if (!array_key_exists('issn', $args) || !array_key_exists('volume', $args) || !array_key_exists('issue', $args) || !array_key_exists('pages', $args)) {
    drupal_goto('expedited_article_delivery/error', array('query' => array('reason' => 'incomplete_data')));
  }
  else if ($user->uid == 0){
    // Catch original link and send users to login so that they bounce back to /expedited_article_delivery/order with the original params
    drupal_goto('expedited_article_delivery/error', array('query' => array('reason' => 'anonymous_user')));
  }
  else if (!expedited_article_delivery_check_roles($user)) {
    drupal_goto('expedited_article_delivery/error', array('query' => array('reason' => 'missing_roles')));
  }
  else {
    echo "Success!";
  }
}

function expedited_article_delivery_error_processor($args) {
  echo "Error page";

  switch ($args['reason']) {

    case 'incomplete_data':
      echo "<p>Incomplete data!</p>";
      break;

    case 'anonymous_user':
      echo "<p>Anonymous user!</p>";
      break;

    case 'missing_roles':
      echo "<p>Missing roles!</p>";
      break;
  }

  drupal_exit();
}

function expedited_article_delivery_check_roles($user) {
  $matches = array_intersect($user->roles, variable_get('expedited_article_delivery_allowed_roles'));
  return count($matches);
}
