function CMain(oData){
    var _bUpdate;
    var _iCurResource = 0;
    var RESOURCE_TO_LOAD = 0;
    var _iState = STATE_LOADING;
    var _oData;
    
    var _oPreloader;
    var _oMenu;
    var _oHelp;
    var _oGame;
    this.name = "";
    this.initContainer = function(){
        var canvas = document.getElementById("canvas");
        s_oStage = new createjs.Stage(canvas);
       
	s_bMobile = jQuery.browser.mobile;
        if(s_bMobile === false){
            s_oStage.enableMouseOver(20);  
            $('body').on('contextmenu', '#canvas', function(e){ return false; });
        }else{
             createjs.Touch.enable(s_oStage);
        }
		
        s_iPrevTime = new Date().getTime();

	createjs.Ticker.addEventListener("tick", this._update);
        createjs.Ticker.setFPS(30);
        
        if(DISABLE_SOUND_MOBILE === false || s_bMobile === false){
            this._initSounds();
        }
        
        s_oSpriteLibrary  = new CSpriteLibrary();

        //ADD PRELOADER
        _oPreloader = new CPreloader();

        this._loadImages();
		
	_bUpdate = true;
        this.initMultiplayerGame_();
        this.s_con_.addEventListener("waitingSlave", Delegate.create(this, this.onWaitngSlave_));
        this.s_con_.addEventListener('allPlayerInfo', Delegate.create(this, this.onAllPlayerInfoReady_));
    };
    
    this.soundLoaded = function(){
         _iCurResource++;

         if(_iCurResource === RESOURCE_TO_LOAD){
             _oPreloader.unload();
            
            if(DISABLE_SOUND_MOBILE === false || s_bMobile === false){
                s_oSoundtrack = createjs.Sound.play("soundtrack",{ loop:100,volume:0.5});
            }

            var roomId = this.getQueryStringRegExp('roomId');
            this.gotoMenu(null,null,roomId);
         }
    };
    
    this._initSounds = function(){
         if (!createjs.Sound.initializeDefaultPlugins()) {
             return;
         }

        if(navigator.userAgent.indexOf("Opera")>0 || navigator.userAgent.indexOf("OPR")>0){
                createjs.Sound.alternateExtensions = ["mp3"];
                createjs.Sound.addEventListener("fileload", createjs.proxy(this.soundLoaded, this));

                createjs.Sound.registerSound("./sounds/click.ogg", "click");
                createjs.Sound.registerSound("./sounds/gameover.ogg", "gameover");
                createjs.Sound.registerSound("./sounds/right.ogg", "right");
                createjs.Sound.registerSound("./sounds/wrong.ogg", "wrong");
                createjs.Sound.registerSound("./sounds/soundtrack.ogg", "soundtrack");
        }else{
                createjs.Sound.alternateExtensions = ["ogg"];
                createjs.Sound.addEventListener("fileload", createjs.proxy(this.soundLoaded, this));

                createjs.Sound.registerSound("./sounds/click.mp3", "click");
                createjs.Sound.registerSound("./sounds/gameover.mp3", "gameover");
                createjs.Sound.registerSound("./sounds/right.mp3", "right");
                createjs.Sound.registerSound("./sounds/wrong.mp3", "wrong");
                createjs.Sound.registerSound("./sounds/soundtrack.mp3", "soundtrack");
        }
        
        RESOURCE_TO_LOAD += 5;
        
    };

    this._loadImages = function(){
        s_oSpriteLibrary.init( this._onImagesLoaded,this._onAllImagesLoaded, this );

        s_oSpriteLibrary.addSprite("but_play","./sprites/but_play.png");
        s_oSpriteLibrary.addSprite("but_exit","./sprites/but_exit.png");
        s_oSpriteLibrary.addSprite("conn_exit","./sprites/conn_exit.png");
        s_oSpriteLibrary.addSprite("bg_menu","./sprites/bg_menu.jpg");
        s_oSpriteLibrary.addSprite("bg_game","./sprites/bg_game.jpg");
        s_oSpriteLibrary.addSprite("gameover_bg","./sprites/gameover_bg.png");
        s_oSpriteLibrary.addSprite("audio_icon","./sprites/audio_icon.png");
        s_oSpriteLibrary.addSprite("toggle_lang","./sprites/toggle_lang.png");
        s_oSpriteLibrary.addSprite("bg_language_selection","./sprites/bg_language_selection.jpg");
        s_oSpriteLibrary.addSprite("cell_bg","./sprites/cell_bg.png");
        s_oSpriteLibrary.addSprite("cell_right","./sprites/cell_right.png");
        s_oSpriteLibrary.addSprite("cell_select_bg","./sprites/cell_select_bg.png");
        s_oSpriteLibrary.addSprite("cell_wrong","./sprites/cell_wrong.png");
        s_oSpriteLibrary.addSprite("hit_area_cell","./sprites/hit_area_cell.png");
        s_oSpriteLibrary.addSprite("timebar_bg","./sprites/timebar_bg.png");
        s_oSpriteLibrary.addSprite("timebar_fill","./sprites/timebar_fill.png");
        s_oSpriteLibrary.addSprite("help_bg","./sprites/help_bg.png");
        s_oSpriteLibrary.addSprite("online_panel","./sprites/online_panel.png");
        s_oSpriteLibrary.addSprite("avatar4","./sprites/avatar4.jpg");
        s_oSpriteLibrary.addSprite("avatar5","./sprites/avatar5.jpg");

        RESOURCE_TO_LOAD += s_oSpriteLibrary.getNumSprites();
        s_oSpriteLibrary.loadSprites();
    };
    
    this._onImagesLoaded = function(){
        _iCurResource++;
        var iPerc = Math.floor(_iCurResource/RESOURCE_TO_LOAD *100);

        _oPreloader.refreshLoader(iPerc);
        
        if(_iCurResource === RESOURCE_TO_LOAD){
            _oPreloader.unload();
            
            if(DISABLE_SOUND_MOBILE === false || s_bMobile === false){
                s_oSoundtrack = createjs.Sound.play("soundtrack",{ loop:100,volume:0.5});
            }
            var roomId = this.getQueryStringRegExp('roomId');
            this.gotoMenu(null,null, roomId);
        }
    };
    
    this._onAllImagesLoaded = function(){
        
    };
    
    this.onAllPreloaderImagesLoaded = function(){
        this._loadImages();
    };
    
    this.setLanguage = function(szLang){
        s_szLangSelected = szLang;

        switch(szLang){
            case LABEL_LANG_0:{
                s_oWordListSelected = new CEngWordlist();
                break;
            }
            
            case LABEL_LANG_1:{
                s_oWordListSelected = new CGerWordlist();
                break;
            }
            
            case LABEL_LANG_2:{
                s_oWordListSelected = new CSpaWordlist();
                break;
            }

            case LABEL_LANG_3:{
                s_oWordListSelected = new CFraWordlist();
                break;
            }
            case LABEL_LANG_4:{
                s_oWordListSelected = new CItaWordlist();
                break;
            }

            case LABEL_LANG_5:{
                s_oWordListSelected = new CPorWordlist();
                break;
            }  
        }

        this.gotoHelp();
    };
    
    this.gotoMenu = function(isReplay, data, roomId){
        _oMenu = new CMenu(roomId);
        _iState = STATE_MENU;
        if(isReplay) {
            if(s_isSinglePlayer){
                _oMenu.onStartSingleGame_();
            }else{
                _oMenu.readyToPlay(data);
            }
        }
    };

    this.gotoGame = function(){
        _oGame = new CGame(_oData, null);
							
        _iState = STATE_GAME;
        
        //$(s_oMain).trigger("game_start");
    };
    
    this.gotoHelp = function(){
        _oHelp = new CHelpPanel();
        _iState = STATE_HELP;
    };
	
    this.stopUpdate = function(){
        _bUpdate = false;
    };

    this.startUpdate = function(){
            _bUpdate = true;
    };
    
    this._update = function(event){
        if(_bUpdate === false){
                return;
        }
        
        var iCurTime = new Date().getTime();
        s_iTimeElaps = iCurTime - s_iPrevTime;
        s_iCntTime += s_iTimeElaps;
        s_iCntFps++;
        s_iPrevTime = iCurTime;
        
        if ( s_iCntTime >= 1000 ){
            s_iCurFps = s_iCntFps;
            s_iCntTime-=1000;
            s_iCntFps = 0;
        }
        
        if(_iState === STATE_GAME){
            _oGame.update();
        }
        
        s_oStage.update(event);

    };

    this.initMultiplayerGame_ = function(){
        this.s_con_ = new GameSocketController();
        this.s_con_.init(appConfig.gameServer.url, {port: appConfig.gameServer.port});
    };

    this.onWaitngSlave_ = function(e){
        if(this.name == 'slave'){
            _oMenu.unload();
            this.setLanguage(e.body.language);
            _oGame = new CGame(_oData, e.body.board);

            _iState = STATE_GAME;
        }
    };

    this.onAllPlayerInfoReady_ = function(e) {
        this.playerInfoArray_ = e.body;
        if(!this.playerInfoArray_[1]) {
            this.playerInfoArray_[1] = {name: TEXT_NULL_PLAYER_NAME, avatar: './sprites/avatar4.jpg', seat: TEXT_NULL_PLAYER_NAME}
        }
    };
    this.getQueryStringRegExp = function(name) { 
        var reg = new RegExp("(^|\\?|&)"+ name +"=([^&]*)(\\s|&|$)", "i"); 
        if (reg.test(location.href)) return unescape(RegExp.$2.replace(/\+/g, " ")); return ""; 
    }; 
    this.playerInfoArray_ = [];
    s_oMain = this;

    _oData = oData;

    this.initContainer();
}
var s_bMobile;
var s_bAudioActive = true;
var s_iCntTime = 0;
var s_iTimeElaps = 0;
var s_iPrevTime = 0;
var s_iCntFps = 0;
var s_iCurFps = 0;
var s_szLangSelected;
var s_oWordListSelected;

var s_oDrawLayer;
var s_oStage;
var s_oMain;
var s_oSpriteLibrary;
var s_oSoundTrack;

var s_isSinglePlayer = false;