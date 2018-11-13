<?php

function expedited_article_delivery_order_processor($args) {
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
  drupal_exit(); 
}
