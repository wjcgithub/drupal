<?php
/**
 * @file
 * advertising_custom_type.features.inc
 */

/**
 * Implements hook_default_advertising_type().
 */
function advertising_custom_type_default_advertising_type() {
  $items = array();
  $items['advertising_heart'] = entity_import('advertising_type', '{
    "type" : "advertising_heart",
    "label" : "\\u5fc3\\u810f\\u5728\\u7ebf\\u5e7f\\u544a\\u4f4d",
    "weight" : "0",
    "data" : "",
    "rdf_mapping" : []
  }');
  return $items;
}
