<?php
/**
 * @param $chatarr(uid,chatname,token)
 *
 */
function createChat($chatarr){
  extract($chatarr);
  $newChat = new customChat();
  $newChat->set('uid',(isset($uid) && is_numeric($uid)) ? $uid : $GLOBALS['user']->uid);
  $newChat->set('title',$chatname);
  $newChat->set('token',$token);
  $newChat->set('public',true);
  $newChat->set('format','plain_text');
  $newChat->save();
  return $newChat->loadChatByToken();
}

/**
 * load chat by token
 * @param $token
 */
function getChatByToken($token){
  $newChat = new customChat();
  $newChat->set('token',$token);
  return $newChat->loadChatByToken();
}

/**
 * return a chat html
 * @param $cid 
 * chat id  
 */
function customGetChat($cid){
  return _custom_chat_get_room($cid);
}

/**
 * return a chat html
 * @param $token 
 */
function getChatroomByToken($token){
  $chat = getChatByToken($token);
  return is_object($chat) ? _custom_chat_get_room($chat->cid) : FALSE;
}

function createChatUser($cid,$uidarr=array()){
  $chatRoom = new customChat($cid);
  if(!empty($uidarr)){
    foreach ($uidarr as $uid) {
      if(is_numeric($uid)){
        $userobj = user_load($uid);
        if(empty($userobj)){
          watchdog('customchat','对不起，没有该用户') ;
        }
        if($chatRoom->checkUserForCid($uid)){
          watchdog('customchat','对不起，该用户已经在这个房间了') ;
        }
        $chatRoom->createChatUsers($uid);
      }
    }
  }
}