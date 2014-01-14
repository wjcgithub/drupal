<?php

/**
 * This field handler aggregates operations that can be done on a advertising
 * under a single field providing a more flexible way to present them in a view
 */
class advertising_handler_advertising_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['advertising_id'] = 'advertising_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('advertising', 'admin/content/advertisings/advertising', array($this->get_value($values, 'advertising_id')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
