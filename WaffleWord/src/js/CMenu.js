function CMenu(roomId){
    var _oBg;
    var _oButPlay;
    var _oConnExit;
    var _oSingleButPlay;
    var _oButAckMulti;
    var _oFade;
    var _oLanguageSelection;
    var _oAudioToggle;
    var _msgPanel;
    var _onlineText;
    var _readyTimeText;
    var _readyNumberText;
    var _roomId = roomId;

    this._init = function(){
        this.block = false;
        _oBg = new createjs.Bitmap(s_oSpriteLibrary.getSprite('bg_menu'));
        var _oConnExitSp = s_oSpriteLibrary.getSprite('conn_exit');

        _msgPanel = new createjs.Container();
        _msgPanel.set({ visible:false, x: (CANVAS_WIDTH/2 - 220), y: (CANVAS_HEIGHT -230) });

        var panel = new createjs.Bitmap(s_oSpriteLibrary.getSprite('online_panel'));
        panel.set({scaleX:1.5, scaleY:1.5});

        _onlineText = new createjs.Text(TEXT_ONLINE_HOST,"25px dimboregular", "#FFCC00");
        _onlineText.set({x: 215, y:50});
        _onlineText.textAlign = "center";
        _onlineText.textBaseline = "alphabetic";
        _onlineText.shadow = new createjs.Shadow("#000", 2, 2, 2);

        _readyTimeText = new createjs.Text("","18px dimboregular", "#FFCC00");
        _readyTimeText.set({ x: 50, y: 120 });
        _readyTimeText.textBaseline = "alphabetic";
        _readyTimeText.shadow = new createjs.Shadow("#000", 2, 2, 2);

        _readyNumberText = new createjs.Text("","18px dimboregular", "#FFCC00");
        _readyNumberText.set({x: 250, y: 120 });
        _readyNumberText.textBaseline = "alphabetic";
        _readyNumberText.shadow = new createjs.Shadow("#000", 2, 2, 2);

        _oConnExit = new CGfxButton(panel.image.width + 135,15,_oConnExitSp);
        _oConnExit.addEventListener(ON_MOUSE_UP, this._onCancelReady, this);

        _msgPanel.addChild(panel, _onlineText, _readyTimeText, _readyNumberText, _oConnExit.getSprite());



        s_oStage.addChild(_oBg);

        var oSprite = s_oSpriteLibrary.getSprite('but_play');
        _oButPlay = new CTextButton((CANVAS_WIDTH/2 + 150),CANVAS_HEIGHT -80,oSprite,TEXT_ONLINE_PLAY,"dimboregular","#ffffff",32);
        _oSingleButPlay = new CTextButton((CANVAS_WIDTH/2 -150),CANVAS_HEIGHT -80,oSprite,TEXT_SINGLE_PLAY,"dimboregular","#ffffff",32);

        _oButPlay.addEventListener(ON_MOUSE_UP, this._onButPlayRelease, this);
        _oSingleButPlay.addEventListener(ON_MOUSE_UP, this._onSingleButPlayRelease, this);

        _oLanguageSelection = new CLanguageSelection();
        
        if(DISABLE_SOUND_MOBILE === false || s_bMobile === false){
            oSprite = s_oSpriteLibrary.getSprite('audio_icon');
            _oAudioToggle = new CToggle(CANVAS_WIDTH - (oSprite.height/2) - 10,(oSprite.height/2) + 10,oSprite,s_bAudioActive,true);
            _oAudioToggle.addEventListener(ON_MOUSE_UP, this._onAudioToggle, this);
        }

        s_oStage.addChild(_msgPanel);
        oSprite = s_oSpriteLibrary.getSprite('but_play');
        _oButAckMulti = new CTextButton((CANVAS_WIDTH/2),CANVAS_HEIGHT -80,oSprite,'OK',"dimboregular","#ffffff",52);
        _oButAckMulti.addEventListener(ON_MOUSE_UP, this._onAckMultiRelease, this);
        _oButAckMulti.setVisible(false);


        _oFade = new createjs.Shape();
        _oFade.graphics.beginFill("black").drawRect(0,0,CANVAS_WIDTH,CANVAS_HEIGHT);
        
        s_oStage.addChild(_oFade);
        
        createjs.Tween.get(_oFade).to({alpha:0}, 1000).call(function(){_oFade.visible = false;});

        /*--------------------add con evet listener*/
        s_oMain.s_con_.addEventListener("waitingMakeRaw",Delegate.create(this, this.onWaitngMakeRaw_));
        s_oMain.s_con_.addEventListener("networkWaiting",Delegate.create(this, this.onNetworkWaiting_));
        s_oMain.s_con_.addEventListener("startSingleGame", Delegate.create(this, this.onStartSingleGame_));
        s_oMain.s_con_.addEventListener("playerDisconnect", Delegate.create(this, this.onHostDisconnect_));
        s_oMain.s_con_.addEventListener("replayNetworkWaiting", Delegate.create(this, this.onReplayNetworkWaiting_));
        if(_roomId){
            this._onButPlayRelease();
        }
    };
    
    this.unload = function(){
        _oButPlay.unload(); 
        _oButPlay = null;
        
        if(DISABLE_SOUND_MOBILE === false || s_bMobile === false){
            _oAudioToggle.unload();
            _oAudioToggle = null;
        }
        
        s_oStage.removeAllChildren();
        _oBg = null;
    };

    this._onCancelReady = function(){
        s_oMain.s_con_.cancelReady();
        this.stopReadyActionTimeout_();
        this.stopReplayReadyActionTimeout_();
        this.hideMsgPanel_();
        _readyTimeText.text = "";
        _readyNumberText.text = "";

    };
    this.readyToPlay = function(data) {
        this.onPlayerInfoReady();
        this.onPlayerReadyToPlay(data);
        this.showMsgPanel_();
    };

    this.showMsgPanel_ = function(){
        _msgPanel.visible = true;
        this.block = true;
    };

    this.hideMsgPanel_ = function(){
        _msgPanel.visible = false;
        this.block = false;
    };

    this._onAudioToggle = function(){
        createjs.Sound.setMute(s_bAudioActive);
        s_bAudioActive = !s_bAudioActive;
    };
    
    this._onButPlayRelease = function(){
       // _oLanguageSelection.show();
        if(this.block){
            return;
        }

        s_isSinglePlayer = false;
        this.readyToPlay({vsInfo: null});
    };

    this._onSingleButPlayRelease = function(){
        if(this.block){
            return;
        }
        this.onStartSingleGame_();
    };

    this._onAckMultiRelease = function(){
        _oButAckMulti.setVisible(false);
        _msgPanel.visible = false;
        _oLanguageSelection.show();
    };

    this.onPlayerReadyToPlay = function(data){
        if(_roomId){data['roomId'] = _roomId;}
        s_oMain.s_con_.readyToPlay(data);
    };

    this.onPlayerInfoReady = function(){
        var guestInfo =(appConfig.hudAPI && appConfig.hudAPI.getUserInfo())?appConfig.hudAPI.getUserInfo():{name: 'player'+BOK.randN(1000),seat: '', avatar:'sprites/avatar5.jpg'};

        s_oMain.s_con_.onePlayerInfoReady({name: guestInfo.name, seat: guestInfo.seat, avatar: guestInfo.avatar});
    };
    this.onWaitngMakeRaw_ = function(e){
        _oConnExit.getSprite().visible = false;
        if(e.body.type == 'host'){
            s_oMain.name = 'host';
            _readyTimeText.visible = false;
            _readyNumberText.visible = false;
            s_isSinglePlayer = s_oMain.playerInfoArray_[1].name == TEXT_NULL_PLAYER_NAME && s_oMain.playerInfoArray_[1].seat == TEXT_NULL_PLAYER_NAME;
            if(s_isSinglePlayer){
                 _onlineText.text = 'You will start single player game!\n Press OK to start the game.';
            }else{
                _onlineText.text = 'Matched with '+ s_oMain.playerInfoArray_[1].name + '@' + s_oMain.playerInfoArray_[1].seat + '\n Press OK to start the game.';
            }
            
            _oButAckMulti.setVisible(true);
        }else{
            s_oMain.name = 'slave';
            _onlineText.text = 'Matched with '+ s_oMain.playerInfoArray_[0].name + '@' + s_oMain.playerInfoArray_[0].seat + '\n Waiting for host to start the game.';
        }
    };


    this.onStartSingleGame_ = function(){
        _oLanguageSelection.show();
        _msgPanel.visible = false;
        s_isSinglePlayer = true;
        s_oMain.playerInfoArray_[0] = {name:  appConfig.hudAPI ? appConfig.hudAPI.getUserName() : 'Player'+BOK.randN(1000) , avatar: appConfig.hudAPI ? appConfig.hudAPI.getUserInfo().avatar : './sprites/avatar4.jpg', seat:  appConfig.hudAPI ? appConfig.hudAPI.getUserInfo().seat : TEXT_NULL_PLAYER_NAME}
    };

    this.onHostDisconnect_ = function(){
        _oLanguageSelection.show();
        _msgPanel.visible = false;
    };
    this.onNetworkWaiting_ = function(msg) {
        this.setWaitingNum(msg.body.waitingNum);
        if(msg.body.readyDuration != null && this.actionTimeoutId_ == null){
            this.setReadydDuration(msg.body.readyDuration);
            this.stopReadyActionTimeout_();
            this.durationNum_ =  Math.round(msg.body.readyDuration / 1000);
            this.readyActionTimeout_();
        }
    };
    this.onReplayNetworkWaiting_ = function(msg) {
        _readyNumberText.visible = false;
        _onlineText.text = 'Waiting ' + msg.body.oppName + ' join game!'
        if(msg.body.readyDuration != null && this.replayActionTimeoutId_ == null){
            this.setReadydDuration(msg.body.readyDuration);
            this.stopReplayReadyActionTimeout_();
            this.durationReadyNum_ =  Math.round(msg.body.readyDuration / 1000);
            this.replayReadyActionTimeout_();
        }
    };

    this.setReadydDuration = function(duration) {
        _readyTimeText.text = TEXT_ONLINE_WAIT_DURATION + duration;
    };
    this.setWaitingNum = function(waitingNum) {
        _readyNumberText.text = TEXT_ONLINE_WAIT_NUMBER + waitingNum;
    };

    /**
     * player ready timeout start
     **/
    this.readyActionTimeout_ = function(){
        if(this.durationNum_ <= 0){
            this.stopReadyActionTimeout_();
            return;
        }
        this.setReadydDuration(this.durationNum_);
        this.durationNum_ --;
        this.actionTimeoutId_ = setTimeout(Delegate.create(this, this.readyActionTimeout_),1000);
    };
    /**
     * player ready timeout stop
     * */
   this.stopReadyActionTimeout_ = function() {
        if(this.actionTimeoutId_) {
            clearTimeout(this.actionTimeoutId_);
            this.actionTimeoutId_ = null;
        }
    };

    /**
     * player ready timeout start
     **/
    this.replayReadyActionTimeout_ = function(){
        if(this.durationReadyNum_ <= 0){
            this.stopReplayReadyActionTimeout_();
            return;
        }
        this.setReadydDuration(this.durationReadyNum_);
        this.durationReadyNum_ --;
        this.replayActionTimeoutId_ = setTimeout(Delegate.create(this, this.replayReadyActionTimeout_),1000);
    };
    /**
     * player ready timeout stop
     * */
   this.stopReplayReadyActionTimeout_ = function() {
        if(this.replayActionTimeoutId_) {
            clearTimeout(this.replayActionTimeoutId_);
            this.replayActionTimeoutId_ = null;
        }
    };
    s_oMenu = this;
        
    this._init();
}
var s_oMenu;
