<?php

/**
 * @file
 * customChat class definition.
 */

/**
 * Represents a customChat.
 */
class customChat {

  protected $changedAttributes = array();

  /**
   * Format to be used when filtering chat message output.
   */
  protected $format = '';

  /**
   * List of user uids allowed in this chat.
   */
  protected $allowedUids = array();

  protected $archived = 0;

  protected $cid = 0;

  protected $uid = 0;

  /*need add to custom room*/
  protected $joinuid = 0;

  protected $token = '';

  protected $created = '';

  protected $public = FALSE;

  protected $title = '';

  protected $latestMessage = FALSE;

  protected $onlineUsers = array();

  public function __construct($cid = FALSE) {
    if ($cid) {
      $this->load($cid);
    }
    $this->created = time();
  }

  public function load($cid) {
    $data = db_query('SELECT * FROM {custom_chat} WHERE cid = :cid', array(':cid' => $cid))->fetchObject();
    if ($data) {
      $this->cid = $data->cid;
      $this->public = $data->public;
      $this->archived = $data->archived;
      $this->title = $data->title;
      $this->token = $data->token;
      $this->created = $data->created;
      $this->format = $data->format ? $data->format : filter_fallback_format();
      $this->loadAllowedUids();
      $this->loadLatestMessage();
    }
  }

  public function createChatUsers($joinuid,$read_only=0) {
    $transaction = db_transaction();
    try {
      $this->joinuid = $joinuid;
      db_merge('custom_chat_user')
        ->key(array('cid' => $this->cid,'uid' => $this->joinuid))
        ->fields(array(
            'cid' => $this->cid,
            'uid' => $this->joinuid,
            'read_only' => $read_only,
            'status' => 1,
        ))
        ->execute();
      if ($this->hasChanged('allowedUids')) {
        $this->updateUids();
      }
      // module_invoke_all('custom_chat_' . $op, $this);
      return TRUE;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('custom_chat_user_save', $e);
      return FALSE;
    }
  }

  public function set($key, $value) {
    if (isset($this->$key) && $this->$key !== $value) {
      $this->$key = $value;
      if (!in_array($key, $this->changedAttributes)) {
        $this->changedAttributes[] = $key;
      }
    }
  }

  public function get($key) {
    if (isset($this->$key)) {
      return $this->$key;
    }
  }

  protected function loadLatestMessage() {
    $messages = $this->loadLatestMessages(1);
    $this->latestMessage = $messages ? current($messages) : FALSE;
    return $this->latestMessage;
  }

  protected function loadAllowedUids() {
    $this->allowedUids = array();
    $uids = db_query('SELECT uid, read_only FROM {custom_chat_user} WHERE cid = :cid', array(':cid' => $this->cid))->fetchAll();
    foreach ($uids as $uid) {
      $this->allowedUids[] = $uid;
    }
    return $this->allowedUids;
  }

  /**
   * Write a message into a chat, and allow any interested modules to react.
   *
   * @param array $message
   * @return boolean
   */
  public function saveMessage($message) {
    drupal_alter('custom_chat_msg', $message);
    if (drupal_write_record('custom_chat_msg', $message)) {
      $this->latestMessage = (object) $message;
      foreach (module_implements('chat_msg_saved') as $module) {
        $function = $module . '_chat_msg_saved';
        $function($message, $this);
      }
      return $message['cmid'];
    }
    return FALSE;
  }

  /**
   * Check to see if the given user has access to this chat.
   *
   * @param mixed $account
   * @param mixed $access_type
   * @return boolean
   */
  public function userHasAccess($account, $access_type) {
    // Allow modules to take over access to custom_chats. To avoid ambiguity,
    // if any modules returns TRUE or FALSE rather than NULL, they win.
    foreach (module_implements('custom_chat_access') as $module) {
      $function = $module . '_custom_chat_access';
      $access = $function($this, $account, $access_type);
      if ($access === TRUE || $access === FALSE) {
        return $access;
      }
    }

    // No module cares about access to this custom_chat, run our standard checks.
    $admin_role_rid = variable_get('user_admin_role', FALSE);
    if ($account->uid == 1 || ($admin_role_rid && isset($account->roles[$admin_role_rid]))) {
      return TRUE;
    }

    $params = array(
      ':cid' => $this->get('cid'),
      ':uid' => $account->uid,
    );
    $sql = 'SELECT cid, read_only FROM {custom_chat_user} WHERE cid = :cid AND uid = :uid';
    if ($user_access = db_query($sql, $params)->fetchObject()) {
      if (!$user_access->read_only || $access_type == 'read') {
        return TRUE;
      }
    }

    if ($access_type == 'read' && ($this->get('public') || user_access('access all custom_chat', $account))) {
      return TRUE;
    }
  }

/**
 * @param $uid
 * check user is not in chatroom
 */
  public function checkUserForCid($uid){
    $params = array(
      ':cid' => $this->get('cid'),
      ':uid' => $uid,
    );
    $sql = 'SELECT cid, read_only FROM {custom_chat_user} WHERE cid = :cid AND uid = :uid';
    if (db_query($sql, $params)->fetchObject()) {
      return true;
    } else {
      return false;
    }
  }

  /**
   * Make the given chat private.
   *
   * @return boolean
   */
  public function makePrivate() {
    return db_query('UPDATE {custom_chat} SET public = 0 WHERE cid = :cid', array(':cid' => $this->get('cid')));
  }

  /**
   * Remove a user from a private chat.
   *
   * @param mixed $user_to_be_removed
   * @param mixed $user_doing_the_removing
   * @return boolean
   */
  public function removeUser($uid) {
    $params = array(
      ':cid' => $this->get('cid'),
      ':uid' => $uid,
    );
    return db_query('DELETE FROM {custom_chat_user} WHERE cid = :cid AND uid = :uid', $params);
  }

  /**
   * Make the given chat public.
   *
   * @return boolean
   */
  public function makePublic() {
    return db_query('UPDATE {custom_chat} SET public = 1 WHERE cid = :cid', array(':cid' => $this->get('cid')));
  }

  protected function hasChanged($attribute) {
    return in_array($attribute, $this->changedAttributes);
  }

  /**
   * Update the uids allowed to access a chat.
   *
   * @param mixed $uids
   * @return void
   */
  protected function updateUids() {
    $sql = "DELETE
            FROM {custom_chat_user}
            WHERE cid = :cid";
    db_query($sql, array(':cid' => $this->get('cid')));

    foreach ($this->allowedUids as $uid) {
      $data = array('uid' => $uid->uid, 'cid' => $this->get('cid'), 'read_only' => $uid->read_only);
      drupal_write_record('custom_chat_user', $data);
    }
  }

  /**
   * Get the message count for a chat.
   */
  public function getMessageCount() {
    $sql = 'SELECT COUNT(cmid) FROM {custom_chat_message} WHERE cid = :cid';
    return db_query($sql, array(':cid' => $this->get('cid')))->fetchField();
  }

  public function save() {
    $transaction = db_transaction();
    try {
      $data = array(
        'title' => $this->title,
        'public' => $this->public,
        'uid' => $this->uid,
        'token' => $this->token,
        'archived' => $this->archived,
        'format' => $this->format,
        'created' => $this->created,
      );
      $op = $this->cid ? 'update' : 'insert';
      if ($op == 'update') {
        $data['cid'] = $this->cid;
        drupal_write_record('custom_chat', $data, 'cid');
      }
      else {
        drupal_write_record('custom_chat', $data);
        $this->cid = $data['cid'];
      }
      if ($this->hasChanged('allowedUids')) {
        $this->updateUids();
      }
      module_invoke_all('custom_chat_' . $op, $this);
      return TRUE;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('custom_chat_save', $e);
      return FALSE;
    }
  }

  /**
   * Get messages for a given chat.
   *
   * @param $last_cmid
   *   Only load messages with cmids greater than this value.
   * @return array $messages
  */
  public function loadMessages($last_cmid = 0,$recipient_uid=NULL) {
    $sql = "SELECT cm.*, u.name, 0 AS guest_id 
            FROM {custom_chat_msg} cm 
            INNER JOIN {users} u ON u.uid = cm.from_uid 
            WHERE cm.cid = :cid 
            AND cm.recipient_uid = :ruid 
            AND cm.cmid > :cmid 
            ORDER BY cm.cmid ASC";
    $r_uid = isset($recipient_uid) ? $recipient_uid : $GLOBALS['user']->uid;
    $args = array(
      ':cid' => $this->cid,
      ':cmid' => $last_cmid,
      ':ruid' => (int)$r_uid
    );
    return $this->loadMessagesHelper($sql, $args);
  }

   /**
   * Get chat cid by token.
   *
   * @param $token
   * 
   * @return array $messages
  */
  public function loadChatByToken() {
    $sql = "SELECT cc.*
            FROM {custom_chat} cc
            WHERE cc.token = :token";
    $args = array(
      ':token' => $this->token,
    );
    return $this->loadChatHelper($sql, $args);
  }

  /**
   * Load the latest messages.
   *
   * @param $limit
   *   Number of messages to load.
   * @return array
   */
  public function loadLatestMessages($limit = 20) {
    if (!preg_match('/^\d+$/', $limit)) {
      return array();
    }
    $sql = "SELECT cm.*, u.name, 0 AS guest_id
            FROM {custom_chat_msg} cm
            INNER JOIN {users} u ON u.uid = cm.from_uid
            WHERE cm.cid = :cid
            ORDER BY cm.cmid DESC
            LIMIT $limit";
    $messages = $this->loadMessagesHelper($sql, array(':cid' => $this->cid));
    return array_reverse($messages, TRUE);
  }

  /**
   * Load older messages.
   *
   * @param $cmid
   *   Only load messages with cmids less than this value.
   * @param limit
   *   The number of messages to return.
   * @return array $messages
   *
   */
  public function loadPreviousMessages($cmid, $limit = 20) {
    $sql = "SELECT cm.*, u.name, 0 AS guest_id
            FROM {custom_chat_msg} cm
            INNER JOIN {users} u ON u.uid = cm.from_uid
            WHERE cm.cid = :cid
            AND cm.cmid < :cmid
            ORDER BY cm.cmid DESC
            LIMIT $limit";
    $args = array(
      ':cid' => $this->cid,
      ':cmid' => $cmid,
    );
    return $this->r($sql, $args);
  }

  /**
   * Load the cmid of the oldest displayed message.
   *
   * @return $cmid
   */
  public function loadPrevMsgId() {
    $messages = $this->loadLatestMessages();
    $message = array_shift($messages);
    if (isset($message->cmid)) {
      return $message->cmid;
    }
    return 0;
  }

  protected function loadMessagesHelper($sql, $args) {
    $messages = array();
    $result = db_query($sql, $args)->fetchAll(PDO::FETCH_OBJ);
    foreach ($result as $message) {
      $messages[$message->cmid] = $message;
    }
    return $messages;
  }

  protected function loadChatHelper($sql, $args) {
    $chats = array();
    $result = db_query($sql, $args)->fetchAll(PDO::FETCH_OBJ);
    if(!empty($result)){
        return $result[0];
    }
    return false;
  }

  /**
   * Why? Because fields caching is not what we want.
   */
  public function __wakeup() {
    $this->load($this->cid);
  }
}
