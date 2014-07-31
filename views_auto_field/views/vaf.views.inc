<?php
function vaf_views_pre_render(&$view){
    cdialog_include();
    drupal_add_js(drupal_get_path('module','vaf').'/js/vaf.js');
}

/**
 * Implements hook_views_data()
 */
function vaf_views_data_alter(&$data) {
	// dpm($data);
  $data['node']['vaf_operation'] = array(
    'field' => array(
      'title' => t('vaf operation'),
      'help' => t('vaf operation button'),
      'handler' => 'vaf_operation',
    ),
  );
}

