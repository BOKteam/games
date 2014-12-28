/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.GameUI");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("quiz.features.game.v.component.Avatar");
goog.require("quiz.features.game.v.component.ProgressBar");
goog.require("quiz.features.game.v.component.OptionButton");
goog.require("quiz.features.game.v.component.QuestionImgPanel");
goog.require("quiz.features.game.v.component.OptionPanel");
goog.require("quiz.features.game.v.component.TimeBar");
goog.require('quiz.AssetsList');

quiz.features.game.v.component.GameUI = GameUI;
BOK.inherits(GameUI, createjs.Container);

function GameUI() {
    createjs.Container.call(this);
     
    this.avatarL_ = new quiz.features.game.v.component.Avatar(CONST.AVATAR.GAME_RADIUS , '#478868', "", 0.4 ); 
    this.avatarR_ = new quiz.features.game.v.component.Avatar(CONST.AVATAR.GAME_RADIUS , '#EF565B', "", 0.4); 

    this.addChild(this.avatarL_, this.avatarR_);

    // init processBar 
    this.processBarL_ = new quiz.features.game.v.component.ProgressBar();
    this.processBarL_.set({x:CONST.GAME_UI.PROGRESS_BAR_OFFSET, y:CONST.GAME_UI.PROGRESS_BAR_Y});

    this.processBarR_ = new quiz.features.game.v.component.ProgressBar();
    this.processBarR_.set({x: CONST.BG.WIDTH -CONST.PROGRESS_BAR.WIDTH - CONST.GAME_UI.PROGRESS_BAR_OFFSET, y:CONST.GAME_UI.PROGRESS_BAR_Y});
    this.addChild(this.processBarL_, this.processBarR_);
    this.question_ = new createjs.Text("Who Invented  \n The Digital Camera?","25px " + CONST.FONT.ALL, "#F8F8F8");
    this.question_.set({x:CONST.BG.WIDTH / 2 , y: CONST.GAME_UI.QUESTION_Y, textAlign: "center", textBaseline:'middle'});

    var questionImg_h = CONST.GAME_UI.QUESTION_IMAGE_HEIGHT;
    var questionImg_w = CONST.BG.WIDTH - CONST.PROGRESS_BAR.WIDTH * 2 - CONST.GAME_UI.PROGRESS_BAR_OFFSET * 2 - CONST.OPTION_PANEL.OPTION_SPACE * 2 ; 

    this.questionImg_ = new quiz.features.game.v.component.QuestionImgPanel(questionImg_w,questionImg_h);

    this.questionImg_.set(GameUI.settings.questionImg);
    var optionWidth = (CONST.BG.WIDTH - CONST.PROGRESS_BAR.WIDTH * 2 - CONST.GAME_UI.PROGRESS_BAR_OFFSET * 2 - CONST.OPTION_PANEL.OPTION_SPACE * (CONST.OPTION_PANEL.OPTION_NUMBER + 1) ) / 2;
    this.optionA_ = new quiz.features.game.v.component.OptionButton(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT, "0");
    this.optionB_ = new quiz.features.game.v.component.OptionButton(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT, "1");

    this.optionC_ = new quiz.features.game.v.component.OptionButton(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT, "2");

    this.optionD_ = new quiz.features.game.v.component.OptionButton(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT, "3");
    this.options_ = [this.optionA_, this.optionB_, this.optionC_, this.optionD_];
    this.addChild(this.optionA_, this.optionB_, this.optionC_, this.optionD_);

    this.playerInfo_ = [
        new createjs.Container(),
        new createjs.Container()
    ];
    this.playerInfo_[0].set(GameUI.settings.playerL);
    this.playerInfo_[1].set(GameUI.settings.playerR);
    this.addChild(this.playerInfo_[0], this.playerInfo_[1]);

    //init profile
    this.profileL_ = {
        nameText : new createjs.Text("","20px " + CONST.FONT.ALL, "#FAFAFA"),
        countryText : new createjs.Text("","10px " + CONST.FONT.ALL, "#5E5E5E"),
        scoreText : new createjs.Text("0","25px " + CONST.FONT.ALL, "#329A69")
    };
    var i = 0;
    BOK.each(this.profileL_, function(profile){
        profile.set({x: 0, y: 25 * i  , textAlign: "left"});
        this.playerInfo_[0].addChild(profile);
        i ++;
    }, this);

    this.scoreTipL_ = new createjs.Text("+0","25px " + CONST.FONT.ALL, "#329A69");
    this.scoreTipL_ .set({x:this.profileL_.scoreText.x ,y: this.profileL_.scoreText.y, alpha: 0 });
    this.playerInfo_[0].addChild(this.scoreTipL_);

    this.profileR_ = {
        nameText : new createjs.Text("","20px " + CONST.FONT.ALL, "#FAFAFA"),
        countryText : new createjs.Text("","10px " + CONST.FONT.ALL, "#5E5E5E"),
        scoreText : new createjs.Text("0","25px " + CONST.FONT.ALL, "#C2494E")
    };
    i =0;
    BOK.each(this.profileR_, function(profile){
        profile.set({x: 0, y: 25 * i , textAlign: "right"});
        this.playerInfo_[1].addChild(profile);
        i ++;
    }, this);
    this.scoreTipR_ = new createjs.Text("+0","25px " + CONST.FONT.ALL, "#329A69");
    this.scoreTipR_ .set({x:this.profileR_.scoreText.x ,y: this.profileR_.scoreText.y, alpha: 0 });
    this.playerInfo_[1].addChild(this.scoreTipR_);


    this.timeBar_ = new quiz.features.game.v.component.TimeBar();
    this.timeBar_.set({x: CONST.BG.WIDTH / 2, y:CONST.GAME_UI.HEAD_HEIGHT / 2});
    this.addChild(this.timeBar_);

    this.addChild(this.question_, this.questionImg_);
}

GameUI.settings = {
    avatarL: {x: CONST.BG.WIDTH / 8, y: CONST.GAME_UI.HEAD_HEIGHT / 2},
    avatarR: {x: 7 * CONST.BG.WIDTH / 8, y:CONST.GAME_UI.HEAD_HEIGHT / 2 },
    playerL: {x:CONST.BG.WIDTH / 4, y: CONST.GAME_UI.HEAD_HEIGHT / 2  - CONST.AVATAR.GAME_RADIUS },
    playerR: {x:3 * CONST.BG.WIDTH / 4, y: CONST.GAME_UI.HEAD_HEIGHT / 2  - CONST.AVATAR.GAME_RADIUS },
    questionImg:{x: CONST.GAME_UI.PROGRESS_BAR_OFFSET + CONST.PROGRESS_BAR.WIDTH + CONST.OPTION_PANEL.OPTION_SPACE, y: CONST.GAME_UI.PROGRESS_BAR_Y},
    option: {x: CONST.GAME_UI.PROGRESS_BAR_OFFSET + CONST.PROGRESS_BAR.WIDTH + CONST.GAME_UI.OPTION_SPACE, y:  CONST.GAME_UI.PROGRESS_BAR_Y + CONST.GAME_UI.QUESTION_IMAGE_HEIGHT + CONST.OPTION_PANEL.OPTION_SPACE}
};
GameUI.prototype.init = function(){
    this.visible = false;
    this.processBarL_.init();
    this.processBarR_.init();
    this.show();
    this.timeBar_.init();
    this.initScore_();
    this.optionA_.visible = false;
    this.optionB_.visible = false;
    this.optionC_.visible =false; 
    this.optionD_.visible = false;
    this.questionImg_.visible =false;
    this.question_.text = "";
};

GameUI.prototype.show = function(){
    this.alpha = 0;
    this.visible = true;    
    createjs.Tween.get(this).to({alpha: 1},CONST.GAME_UI.SWITCH_TIME);
};

GameUI.prototype.hide = function(){
    this.alpha = 1;
    createjs.Tween.get(this).to({alpha: 1},CONST.GAME_UI.SWITCH_TIME);
    this.visible = false;   
};
/**
 * @param {Array} players // the players info : player format: 
 * {
 *  name: {string},
 *  avatar: {string},
 *  country: {string}  
 * }
 */
GameUI.prototype.setPlayersInfo = function(players){
    var player1 = players[0];
    var player2 = players[1];
    this.setProfile_(player1, this.profileL_);
    this.setProfile_(player2, this.profileR_);
    this.setAvatars_(player1.avatar, player2.avatar);

};

GameUI.prototype.setProfile_ = function(player, profile){
    profile.nameText.text = player.name;
    profile.countryText.text = player.country;
};
GameUI.prototype.setAvatars_ = function(avatarL, avatarR){
     //init avatar 
    this.avatarL_.setAvatar(avatarL);
    this.avatarR_.setAvatar(avatarR);
    this.avatarL_.set(GameUI.settings.avatarL); 
    this.avatarR_.set(GameUI.settings.avatarR);
};

/**
 * @param {Object} question // data format:{
        question:{string},
        img:{string},
        choices:{Array}
    }
 */
GameUI.prototype.setQuestion = function(question){
    var self =this;
    this.question_.lineWidth = 350;
    this.question_.text = question.question;
    this.questionImg_.setImage(question.img);
    if(question.img){
        //this.optionPanel_.set(GameUI.settings.option);
        this.optionLayoutDoubleRow();
    }else{
        //this.optionPanel_.set({x: 0, y: 0});
        this.optionLayoutSingleRow();
    }
    BOK.each(this.options_, function(option, index){
        option.text_.text = question.choices[index];
    }, this);
    this.optionA_.visible = true;
    this.optionB_.visible = true;
    this.optionC_.visible =true; 
    this.optionD_.visible = true;
    
};

GameUI.prototype.updateMeScore = function(score, currentScore){
    if(currentScore != 0){
        this.scoreTipL_.text = "+" + currentScore;
        this.scoreTipL_.alpha = 0.8;
        this.scoreTipL_.set({y:this.profileL_.scoreText.y});
        createjs.Tween.get(this.scoreTipL_).to({y: this.scoreTipL_.y - 45,alpha: 0},1000);
        this.profileL_.scoreText.text = score;     
    }
};

GameUI.prototype.updateRScore = function(score, currentScore){
    if(currentScore != 0){
        this.scoreTipR_.text = "+" + currentScore;
        this.scoreTipR_.alpha = 0.8;
        this.scoreTipR_.set({y:this.profileR_.scoreText.y});
        createjs.Tween.get(this.scoreTipR_).to({y: this.scoreTipR_.y - 45,alpha: 0},1000);
        this.profileR_.scoreText.text = score;     
    }
};

GameUI.prototype.initScore_  = function(){
    this.profileL_.scoreText.text = 0;
    this.profileR_.scoreText.text = 0;
};

GameUI.prototype.optionLayoutSingleRow = function(){
    var optionWidth = (CONST.BG.WIDTH - CONST.PROGRESS_BAR.WIDTH * 2 - CONST.GAME_UI.PROGRESS_BAR_OFFSET * 2 - CONST.OPTION_PANEL.OPTION_SPACE * 2 ) ; 
    var optionSpace = (CONST.BG.HEIGHT - CONST.GAME_UI.PROGRESS_BAR_Y  - 15 - CONST.OPTION_PANEL.OPTION_SINGLE_HEIGHT * 4) / 3; 

    var optionA_x = CONST.GAME_UI.PROGRESS_BAR_OFFSET + CONST.PROGRESS_BAR.WIDTH + CONST.OPTION_PANEL.OPTION_SPACE;

    var optionA_y = CONST.GAME_UI.PROGRESS_BAR_Y ;

    var optionB_x = optionA_x ;
    var optionB_y = optionA_y + optionSpace + CONST.OPTION_PANEL.OPTION_SINGLE_HEIGHT;

    var optionC_x = optionA_x ;
    var optionC_y = optionB_y + optionSpace + CONST.OPTION_PANEL.OPTION_SINGLE_HEIGHT;

    var optionD_x = optionA_x ;
    var optionD_y = optionC_y + optionSpace + CONST.OPTION_PANEL.OPTION_SINGLE_HEIGHT;

    this.optionA_.setOption(optionWidth, CONST.OPTION_PANEL.OPTION_SINGLE_HEIGHT);
    this.optionB_.setOption(optionWidth, CONST.OPTION_PANEL.OPTION_SINGLE_HEIGHT);
    this.optionC_.setOption(optionWidth, CONST.OPTION_PANEL.OPTION_SINGLE_HEIGHT);
    this.optionD_.setOption(optionWidth, CONST.OPTION_PANEL.OPTION_SINGLE_HEIGHT);

    this.optionA_.set({x: optionA_x, y:optionA_y});
    this.optionB_.set({x: optionB_x , y: optionB_y});
    this.optionC_.set({x: optionC_x, y: optionC_y });
    this.optionD_.set({x: optionD_x , y: optionD_y});
};

GameUI.prototype.optionLayoutDoubleRow = function(){
    var optionWidth = (CONST.BG.WIDTH - CONST.PROGRESS_BAR.WIDTH * 2 - CONST.GAME_UI.PROGRESS_BAR_OFFSET * 2 - CONST.OPTION_PANEL.OPTION_SPACE * (CONST.OPTION_PANEL.OPTION_NUMBER + 1) ) / 2; 
    var optionA_x = CONST.GAME_UI.PROGRESS_BAR_OFFSET + CONST.PROGRESS_BAR.WIDTH + CONST.OPTION_PANEL.OPTION_SPACE;

    var optionA_y = CONST.GAME_UI.PROGRESS_BAR_Y + CONST.GAME_UI.QUESTION_IMAGE_HEIGHT + CONST.OPTION_PANEL.OPTION_SPACE;

    var optionB_x = optionA_x + CONST.OPTION_PANEL.OPTION_SPACE + optionWidth;
    var optionB_y = optionA_y;

    var optionC_x = optionA_x ;
    var optionC_y = optionA_y + CONST.OPTION_PANEL.OPTION_SPACE + CONST.OPTION_PANEL.OPTION_HEIGHT;

    var optionD_x = optionA_x + CONST.OPTION_PANEL.OPTION_SPACE + optionWidth;
    var optionD_y = optionA_y + CONST.OPTION_PANEL.OPTION_SPACE + CONST.OPTION_PANEL.OPTION_HEIGHT;

    this.optionA_.setOption(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT);
    this.optionB_.setOption(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT);
    this.optionC_.setOption(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT);
    this.optionD_.setOption(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT);

    this.optionA_.set({x: optionA_x, y:optionA_y});
    this.optionB_.set({x: optionB_x , y: optionB_y});
    this.optionC_.set({x:optionC_x, y: optionC_y });
    this.optionD_.set({x:optionD_x , y: optionD_y});
};