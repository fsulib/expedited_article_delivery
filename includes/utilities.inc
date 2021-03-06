<?php

function expedited_article_delivery_order_processor($args) {
  global $user;
  global $base_url;
  
  $_SESSION['open_url_fields'] = serialize($args);
  
  if (!expedited_article_delivery_full_metadata($args)) {
    drupal_goto('expedited_article_delivery/error', array('query' => array('reason' => 'incomplete_data')));
  }
  else if (!expedited_article_delivery_date_check($args['date'])) {
    drupal_goto('expedited_article_delivery/error', array('query' => array('reason' => 'out_of_date')));
  }
  else if ($user->uid == 0){
    $timestamp = time();
    // Catch original link and send users to login so that they bounce back to /expedited_article_delivery/order with the original params
    $login_url = variable_get('expedited_article_delivery_login_url'); 
    $redirect_url = $base_url . $_SERVER['REQUEST_URI'];
    $cas_destination_url = '/cas?destination=expedited_article_delivery/redirect/' . $timestamp;
    variable_set($timestamp, $redirect_url);
    drupal_goto($login_url . urlencode($base_url) . urlencode($cas_destination_url));
  }
  else if (!expedited_article_delivery_check_roles($user)) {
    drupal_goto('expedited_article_delivery/error', array('query' => array('reason' => 'missing_roles')));
  }
  else {
    $issns = expedited_article_delivery_get_issns();
    if (!in_array('"'.$args['ISSN'].'"', $issns)) {
      drupal_goto('expedited_article_delivery/error', array('query' => array('reason' => 'issn_not_allowed')));
    } 
    else {
      module_load_include('inc', 'expedited_article_delivery', 'includes/reprints_api');
      expedited_article_delivery_reprints_order($args);
    }
  }
}

function expedited_article_delivery_success_processor($args) {

  $order_info =  array();
  $order_info['order_id'] = $args['order_id'];
  $order_info['rnd_id'] = $args['rnd_id'];
  return $order_info;
  
}

function expedited_article_delivery_full_metadata($args) {
  if(!array_key_exists('ISSN', $args) || !array_key_exists('pages', $args)
      || !array_key_exists('date', $args)) 
  {
    return false;
  } 
  else 
  {
    return true;
  }
}

function expedited_article_delivery_date_check($date) {
  $year = substr($date, 0, 4);
  $month = substr($date, 4, 2);
  $day = substr($date, 6, 2);
  $article_timestamp = mktime(0,0,0,$month,$day,$year);
  
  $now = time();
  $delta = 3600*24*30*60;
  
  if($article_timestamp > ($now-$delta)) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}

function expedited_article_delivery_check_roles($user) {
  $matches = array_intersect($user->roles, variable_get('expedited_article_delivery_allowed_roles'));
  return count($matches);
}

function expedited_article_delivery_get_issns() {
  $gs_key = variable_get('expedited_article_delivery_spreadsheet_key', '');
  $gs_data = file_get_contents("https://docs.google.com/spreadsheets/d/{$gs_key}/gviz/tq?tqx=out:csv");
  return explode("\n", $gs_data);
}
