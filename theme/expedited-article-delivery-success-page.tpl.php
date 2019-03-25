<?php
/**
 * @file
 * Default theme implementation for expedited article delivery success page.
 */
?>

<h2>Order Successfully Processed</h2>

  <p>Your order was successful! You should receive an email within two hours with instructions to download the article you requested.</p>
  <p><strong>Order ID: </strong><?php print $order_id; ?><br />
  <strong>Rnd ID: </strong><?php print $rnd_id; ?></p>
  <?php print $success_message; ?>
  

