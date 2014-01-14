<?php
/**
 * @file
 * advertising_custom_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function advertising_custom_views_views_default_views() {
  $export = array();

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
  $handler->display->display_options['use_more_text'] = '更多';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer advertisings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = '重置';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« 第一页';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ 前一页';
  $handler->display->display_options['pager']['options']['tags']['next'] = '下一页 ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = '末页 »';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* 字段: Advertising: Advertising ID */
  $handler->display->display_options['fields']['advertising_id']['id'] = 'advertising_id';
  $handler->display->display_options['fields']['advertising_id']['table'] = 'advertising';
  $handler->display->display_options['fields']['advertising_id']['field'] = 'advertising_id';
  /* 字段: advertising: 广告位名称 */
  $handler->display->display_options['fields']['field_adv_name']['id'] = 'field_adv_name';
  $handler->display->display_options['fields']['field_adv_name']['table'] = 'field_data_field_adv_name';
  $handler->display->display_options['fields']['field_adv_name']['field'] = 'field_adv_name';
  /* 字段: advertising: 广告位图片样式 */
  $handler->display->display_options['fields']['field_adv_img_style']['id'] = 'field_adv_img_style';
  $handler->display->display_options['fields']['field_adv_img_style']['table'] = 'field_data_field_adv_img_style';
  $handler->display->display_options['fields']['field_adv_img_style']['field'] = 'field_adv_img_style';
  /* 字段: Advertising: 链接 */
  $handler->display->display_options['fields']['link_advertising']['id'] = 'link_advertising';
  $handler->display->display_options['fields']['link_advertising']['table'] = 'advertising';
  $handler->display->display_options['fields']['link_advertising']['field'] = 'link_advertising';
  $handler->display->display_options['fields']['link_advertising']['label'] = '查看';
  /* 字段: Advertising: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'advertising';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/advertisings/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = '广告位列表列表';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'advertising';
  $handler->display->display_options['tab_options']['description'] = 'Manage advertising';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['advertising_manage'] = array(
    t('Master'),
    t('广告位管理'),
    t('更多'),
    t('Apply'),
    t('重置'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« 第一页'),
    t('‹ 前一页'),
    t('下一页 ›'),
    t('末页 »'),
    t('Advertising ID'),
    t('.'),
    t(','),
    t('广告位名称'),
    t('广告位图片样式'),
    t('查看'),
    t('Operations links'),
    t('Page'),
  );
  $export['advertising_manage'] = $view;

  return $export;
}
