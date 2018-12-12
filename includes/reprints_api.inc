<?php

function expedited_article_delivery_reprints_order($args) {
  define ('NS', 'http://reprintsdesk.com/webservices/');
  $reprints_url = NULL;
  $reprints_username = NULL;
  $reprints_password = NULL;

  if(variable_get('expedited_article_delivery_debug')) 
  {
    $reprints_url = variable_get('expedited_article_delivery_test_url');
    $reprints_username = variable_get('expedited_article_delivery_test_username');
    $reprints_password = variable_get('expedited_article_delivery_test_password');
  }
  else 
  {
    $reprints_url = variable_get('expedited_article_delivery_production_url');
    $reprints_username = variable_get('expedited_article_delivery_production_username');
    $reprints_password = variable_get('expedited_article_delivery_production_password');
  }

  // Connect to web service
  $client = new SOAPClient($reprints_url, array('trace' => true));

  // Prepare SoapHeader parameters
  $header_parameters = array(
    'UserName' => $reprints_username,
    'Password' => $reprints_password
  );
  $headers = new SoapHeader(NS, 'UserCredentials', $header_parameters, false);

  // Prepare SOAP Client
  $client->__setSOAPHeaders(array($headers));

  // Create the XML Request
  $xml_request = new XMLWriter();  
  create_xml_request($xml_request, $reprints_username, $args);
  
  // Convert it to a valid SoapVar
  $params = new SoapVar($xml_request->outputMemory(), XSD_ANYXML);

  // Call the CheckAvailability service
  try {
    echo "Contacting Reprints Desk API. Please wait...\n\n";
    $order_result = $client->Order_PlaceOrder2($params);
  } catch (Exception $e) {
    echo "** There was an error. Here is the debugging information. **\n\n";
    echo "Request headers: \n" . $client->__getLastRequestHeaders();
    echo "\n Request body: \n" . $client->__getLastRequest();
    echo "\n Response headers: \n" . $client->__getLastResponseHeaders();
    echo "\n Response body: \n" . $client->__getLastResponse();
    echo "\n\n ** End of debugging information. ** \n\n";
  }

  // Success! Display results
  if($order_result->Order_PlaceOrder2Result == 1) {
    drupal_goto('expedited_article_delivery/success', array('query' => array('order_id' => $order_result->orderID, 'rnd_id' => $order_result->rndID)));      
  } else {
    echo "\n\nThere was an error processing your request.";
  }

  drupal_exit();
}

function create_xml_request(&$xml, $reprints_username, $args) {
  global $user;
  $article_metadata = createMetadataStorage($args);
  $reprints_order_type_id = variable_get('expedited_article_delivery_order_type_id');
  $reprints_delivery_method_id = variable_get('expedited_article_delivery_delivery_method_id');
  $reprints_user_email = variable_get('expedited_article_delivery_user_email');
  $reprints_user_firstname = variable_get('expedited_article_delivery_user_firstname');
  $reprints_user_lastname = variable_get('expedited_article_delivery_user_lastname');

  $xml->openMemory();
  $xml->startElementNS('web','Order_PlaceOrder2', NS);
  $xml->startElementNS('web','xmlNode', NS);
  $xml->startElement('order');

  // Section 1: Order detail
  $xml->startElement('orderdetail');
  addXMLElement($xml, 'ordertypeid', $reprints_order_type_id, FALSE);
  addXMLElement($xml, 'deliverymethodid', $reprints_delivery_method_id, FALSE);
  addXMLElement($xml, 'comment', '');
  addXMLElement($xml, 'aulast', $article_metadata['aulast']); 
  addXMLElement($xml, 'aufirst', $article_metadata['aufirst']);
  addXMLElement($xml, 'issn', $args['issn']);
  addXMLElement($xml, 'eissn', $article_metadata['eissn']);
  addXMLElement($xml, 'isbn', $article_metadata['isbn']);
  addXMLElement($xml, 'title', $article_metadata['title'], '');
  addXMLElement($xml, 'atitle',$article_metadata['atitle'], '');
  addXMLElement($xml, 'volume', $args['volume']);
  addXMLElement($xml, 'issue', $args['issue']);
  addXMLElement($xml, 'spage', $article_metadata['spage']);
  addXMLElement($xml, 'epage', $article_metadata['epage']);
  addXMLElement($xml, 'pages', $args['pages']);
  addXMLElement($xml, 'date', $article_metadata['date']);
  addXMLElement($xml, 'doi', $article_metadata['doi']);
  addXMLElement($xml, 'pubmedid', '');
  $xml->endElement();
  
  // Section 2: User
  $xml->startElement('user');
  addXMLElement($xml, 'username', $reprints_username, FALSE);
  addXMLElement($xml, 'email', $reprints_user_email, FALSE);
  $xml->startElement('billingreference');
  $xml->endElement();
  addXMLElement($xml, 'firstname', $reprints_user_firstname);
  addXMLElement($xml, 'lastname', $reprints_user_lastname);
  $xml->endElement();

  // Section 3: Delivery Profile
  $xml->startElement('deliveryprofile');
  addXMLElement($xml, 'firstname', '*');
  addXMLElement($xml, 'lastname', '*');
  addXMLElement($xml, 'companyname', '*');
  addXMLElement($xml, 'address1', '*');
  $xml->startElement('address2');
  $xml->endElement();
  addXMLElement($xml, 'city', '*');
  addXMLElement($xml, 'statecode', 'FL');
  $xml->startElement('statename');
  $xml->endElement();
  addXMLElement($xml, 'zip', '*');
  addXMLElement($xml, 'countrycode', 'US');
  addXMLElement($xml, 'email', $user->mail, FALSE);
  addXMLElement($xml, 'phone', '*');
  $xml->startElement('fax');
  $xml->endElement();
  $xml->endElement();

  //Section 4: Processing Instructions
  $xml->startElement('processinginstructions');

  $xml->startElement('processinginstruction');
  $xml->writeAttribute('id', 1);
  $xml->writeAttribute('valueid', 1);
  $xml->endElement();

  $xml->endElement();

  //Section 5: Customer References
  $xml->startElement('customerreferences');

  $xml->startElement('customerreference');
  $xml->writeAttribute('id', 1);
  $xml->startCdata();
  $xml->text('ID#1234567');
  $xml->endCdata();
  $xml->endElement();  

  $xml->endElement();

  //End of Sections
  $xml->endElement();
  $xml->endElement();
  $xml->endElement();
}

function addXMLElement(&$xml, $elementName, $text, $cdata = TRUE) {
  $xml->startElement($elementName);

  if($cdata) {
    $xml->startCdata();
  }

  $xml->text($text);

  if($cdata) {
    $xml->endCdata();
  }

  $xml->endElement();
}

function createMetadataStorage($args) {
  $article_metadata = array();
 
  if(array_key_exists('aulast', $args)) {
    $article_metadata['aulast'] = $args['aulast'];
  }
  else {
    $article_metadata['aulast'] = '';
  }
  
  if(array_key_exists('aufirst', $args)) {
    $article_metadata['aufirst'] = $args['aufirst'];
  }
  else {
    $article_metadata['aufirst'] = '';
  }
  
  if(array_key_exists('eissn', $args)) {
    $article_metadata['eissn'] = $args['eissn'];
  }
  else {
    $article_metadata['eissn'] = '';
  }
  
  if(array_key_exists('isbn', $args)) {
    $article_metadata['isbn'] = $args['isbn'];
  }
  else {
    $article_metadata['isbn'] = '';
  }
  
  if(array_key_exists('title', $args)) {
    $article_metadata['title'] = $args['title'];
  }
  else {
    $article_metadata['title'] = '';
  }
  
  if(array_key_exists('atitle', $args)) {
    $article_metadata['atitle'] = $args['atitle'];
  }
  else {
    $article_metadata['atitle'] = '';
  }
  
  if(array_key_exists('spage', $args)) {
    $article_metadata['spage'] = $args['spage'];
  }
  else {
    $article_metadata['spage'] = '';
  }
  
  if(array_key_exists('epage', $args)) {
    $article_metadata['epage'] = $args['epage'];
  }
  else {
    $article_metadata['epage'] = '';
  }
  
  if(array_key_exists('date', $args)) {
    $article_metadata['date'] = $args['date'];
  }
  else {
    $article_metadata['date'] = '';
  }
  
  if(array_key_exists('doi', $args)) {
    $article_metadata['doi'] = $args['doi'];
  }
  else {
    $article_metadata['doi'] = '';
  }
  
  return $article_metadata;
}