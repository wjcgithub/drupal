<?php
/**
 * @file
 * advertising_custom_views.features.inc
 */

/**
 * Implements hook_default_advertising_type().
 */
function advertising_custom_views_default_advertising_type() {
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

/**
 * Implements hook_views_api().
 */
function advertising_custom_views_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}
