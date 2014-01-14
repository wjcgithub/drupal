<?php

/**
 * @file
 * Providing extra functionality for the Advertising UI via views.
 */


/**
 * Implements hook_views_data()
 */
function advertising_views_data_alter(&$data) { 
  $data['advertising']['link_advertising'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the advertising.'),
      'handler' => 'advertising_handler_link_field',
    ),
  );
  $data['advertising']['edit_advertising'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the advertising.'),
      'handler' => 'advertising_handler_edit_link_field',
    ),
  );
  $data['advertising']['delete_advertising'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the advertising.'),
      'handler' => 'advertising_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows advertisings/advertising/%advertising_id/op
  $data['advertising']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this advertising.'),
      'handler' => 'advertising_handler_advertising_operations_field',
    ),
  );
}


/**
 * Implements hook_views_default_views().
 */
function advertising_views_default_views() {
  $view = new view();
  $view->name = 'advertising_manage';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'advertising';
  $view->human_name = '广告位管理';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = '广告位管理';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer advertisings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Advertising: Advertising_id */
  $handler->display->display_options['fields']['advertising_id']['id'] = 'advertising_id';
  $handler->display->display_options['fields']['advertising_id']['table'] = 'advertising';
  $handler->display->display_options['fields']['advertising_id']['field'] = 'advertising_id';
  /* Field: Advertising: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'advertising';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Advertising: Link */
  $handler->display->display_options['fields']['link_advertising']['id'] = 'link_advertising';
  $handler->display->display_options['fields']['link_advertising']['table'] = 'advertising';
  $handler->display->display_options['fields']['link_advertising']['field'] = 'link_advertising';
  /* Field: Advertising: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'advertising';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  /* Filter criterion: Advertising: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'advertising';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'advertising_heart' => 'advertising_heart',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/advertisings';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'adv列表';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'advertising';
  $handler->display->display_options['tab_options']['description'] = 'Manage advertising';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $views[$view->name] = $view;
  return $views;
}
