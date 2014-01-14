<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class advertising_handler_delete_link_field extends advertising_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy advertising to check access against
    $dummy_advertising = (object) array('type' => $type);
    if (!advertising_access('edit', $dummy_advertising)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $advertising_id = $values->{$this->aliases['advertising_id']};
    
    return l($text, 'admin/content/advertisings/advertising/' . $advertising_id . '/delete');
  }
}
