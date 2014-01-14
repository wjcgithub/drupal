<?php
/**
 * 获取根级分类
 */
function getTaxOne(){
    $tax = taxonomy_get_vocabularies();       
        $taxarr = array();
        $taxarr[-1] = '==请选择==';
        $vidarr = array(3,4,11,13,19,21);
        foreach($tax as $k=>$v){
            if(in_array($v->vid, $vidarr)){
                $taxarr[$v->vid] = $v->name;
            }
        }
    return $taxarr;
}

/**
 * 获取子栏目列表
 * parm1 : 
 */
function getTaxChildren(){
    $vid = $_POST['vid'];
    $parentid = $_POST['parentid'];
    $taxarr = taxonomy_get_tree($vid, $parentid, 1);
    $newarr = array();
    if(count($taxarr)>0){
        foreach($taxarr as $k=>$v){
            $newarr[$v->tid] = $v->name;
        }
        $json['ret'] = true;
        $json['html'] = generationSelect($newarr, array($vid));
    }else{
        $json['ret'] = false;
    }
    echo drupal_json_encode($json);
}

function generationSelect($arr,$tags){
    $opts = '';
    foreach ($arr as $tid => $value) {
        $opts .='<option value="'.$tid.'">'.$value.'</option>'; 
    }
    $str  ='<div class="empty">--</div><select class="form-select taxchildren" name="selectt'.implode('_', $tags).'"  id="taxchil'.implode('_', $tags).'"><option value="-1">==请选择==</option>';
    $str .=$opts;
    $str .='</select>';
    return $str;
}

function get_article_term($vid){ 
    switch ($vid) {
        case 1:
            return array('news','meeting_notice','meeting_bulletin','interview','periodical_reading','book');
            break;
        case 7:
            return array('academic','continue_edu','special','specialist_seminar');
            break;
        case 13:
            return array('case','case_case','case_discuss','operation');
            break;
        case 17:
            return array('news','meeting_notice');
            break;
        case 18:
            return array('animation','e_book','guide','images_library','medicate','source_ppt','summarize');
            break;
        case 274:
            return array('column_ppt');
            break;    
        
        default:
            return array();
            break;
    }
}