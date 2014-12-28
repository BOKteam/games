/**
 * Created by Envee.
 *
 * Date: 14-10-28
 * Time: pm 16:08
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.QuizResultUI");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("bok.easelui.EsButton");
goog.require("quiz.features.game.v.component.Avatar");
goog.require("quiz.features.game.v.component.TextButton");
goog.require("quiz.features.game.v.component.PKIcon");

goog.require('quiz.AssetsList');

quiz.features.game.v.component.QuizResultUI = QuizResultUI;
BOK.inherits(QuizResultUI, createjs.Container);

function QuizResultUI(){
	createjs.Container.call(this);
	this.bg_ = new createjs.Shape(new createjs.Graphics().beginFill("#303030").drawRect(0, 0, CONST.BG.WIDTH, CONST.BG.HEIGHT));
	this.addChild(this.bg_);

	this.resultText_ = new createjs.Text("YOU WIN!", "75px " + CONST.FONT.ALL, "#ffffff");
	this.initDownPanel_();
	this.pkPanel_  = new quiz.features.game.v.component.PKIcon(CONST.QUIZ_RESULT.PK_WIDTH);
	this.pkPanel_.set(QuizResultUI.settings.pk);
	this.addChild(this.pkPanel_);
	this.setProfiles_();
	this.setAvatars();

    this.scoreBtns_ = {
        match: new EsButton('30', {font: '30px ' + CONST.FONT.ALL,
            fontColor: '#06A6C6',
            borderColor: '#06A6C6',
            bgColor: '#333',
            border: 2,
            enableShadow: false,
            width: CONST.QUIZ_RESULT.SCORE_WIDTH,
            height: CONST.QUIZ_RESULT.SCORE_HEIGHT}),
        finish: new EsButton('30', {font: '30px ' + CONST.FONT.ALL,
            fontColor: '#FFBE1A',
            borderColor: '#FFBE1A',
            bgColor: '#333',
            border: 2,
            enableShadow: false,
            width: CONST.QUIZ_RESULT.SCORE_WIDTH,
            textOffsetX:-10,
            height: CONST.QUIZ_RESULT.SCORE_HEIGHT}),
        victory: new EsButton('30', {font: '30px ' + CONST.FONT.ALL,
            fontColor: '#01D074',
            borderColor: '#01D074',
            bgColor: '#333',
            border: 2,
            enableShadow: false,
            textOffsetX:-10,
            width: CONST.QUIZ_RESULT.SCORE_WIDTH,
            height: CONST.QUIZ_RESULT.SCORE_HEIGHT}),
        powerup:new EsButton('30', {font: '30px ' + CONST.FONT.ALL,
            fontColor: '#FFBE1A',
            borderColor: '#FFBE1A',
            bgColor: '#333',
            border: 2,
            enableShadow: false,
            width: CONST.QUIZ_RESULT.SCORE_WIDTH,
            height: CONST.QUIZ_RESULT.SCORE_HEIGHT}),
        total: new EsButton('30', {font: '30px ' + CONST.FONT.ALL,
            fontColor: '#9470E0',
            borderColor: '#9470E0',
            bgColor: '#333',
            border: 2,
            enableShadow: false,
            width: CONST.QUIZ_RESULT.SCORE_WIDTH,
            height: CONST.QUIZ_RESULT.SCORE_HEIGHT})
    };

	this.grades_ = {
        match:this.generateScorePanel_('MATCH \nSCORE', this.scoreBtns_.match),
        finish: this.generateScorePanel_('FINISH \nBONUS',this.scoreBtns_.finish),
        victory: this.generateScorePanel_('VICTORY \nBONUS',this.scoreBtns_.victory),
        powerup: this.generateScorePanel_('POWER UP \nBONUS',this.scoreBtns_.powerup),
        total: this.generateScorePanel_('TOTAL',this.scoreBtns_.total)};

    var i = 0;
    BOK.each(this.grades_, function(grade){
    	grade.set({x: 20 + (CONST.QUIZ_RESULT.SCORE_WIDTH + 10)* i, y:CONST.QUIZ_RESULT.SCORE_POSITION_Y})
    	this.addChild(grade);
    	i++;
    }, this);

	this.resultText_.set(QuizResultUI.settings.resultText);
	this.addChild(this.resultText_);

	this.homeBtn_ = new quiz.features.game.v.component.TextButton(CONST.QUIZ_RESULT.OPERATE_BTN_WIDTH, CONST.QUIZ_RESULT.OPERATE_BTN_HEIGHT, "#ffffff", 4, "HOME", "30px " + CONST.FONT.ALL, "#DD853B", "center");
	this.rematchBtn_ = new quiz.features.game.v.component.TextButton(CONST.QUIZ_RESULT.OPERATE_BTN_WIDTH, CONST.QUIZ_RESULT.OPERATE_BTN_HEIGHT, "#ffffff", 4, "Rematch", "30px " + CONST.FONT.ALL, "#FF8787", "center");
	this.homeBtn_.set(QuizResultUI.settings.homeBtn);
	this.rematchBtn_.set(QuizResultUI.settings.rematchBtn);
	this.addChild(this.homeBtn_, this.rematchBtn_);
}

QuizResultUI.prototype.generateScorePanel_ = function(title, btn){
	var panel = new createjs.Container();
	var title = new createjs.Text(title, "15px " + CONST.FONT.ALL, "#fff")
	title.set({x:CONST.QUIZ_RESULT.SCORE_WIDTH / 2,textAlign: 'center'})
	btn.set({y: 40})
	panel.addChild(title, btn);
	return panel;
};

QuizResultUI.prototype.show = function(){
	this.alpha = 0;
	this.visible = true;	
	createjs.Tween.get(this).to({alpha: 1},CONST.QUIZ_RESULT.SWITCH_TIME);
};

QuizResultUI.prototype.hide = function(){
	this.alpha = 1;
	createjs.Tween.get(this).to({alpha: 1},CONST.QUIZ_RESULT.SWITCH_TIME);
	this.visible = false;	
};

QuizResultUI.prototype.init =  function(scores, players, result){
	this.setPlayerInfos(players);
	this.show();
	this.setScore_(scores);
	this.setResult_(result);
};

/**
 *
 * @param {Object} scores // the result score , data format: {
 *   match: {number},
 *   history: {number},
 *   victory: {number}
 *   powerup: {number}
 *   total: {number}
 * }
 * @private
 */
QuizResultUI.prototype.setScore_ = function(scores){
    this.scoreBtns_.match.textDisplay_.text = scores.match;
    this.scoreBtns_.finish.textDisplay_.text = '+' + scores.finish;
    this.scoreBtns_.victory.textDisplay_.text = '+' + scores.victory;
    this.scoreBtns_.powerup.textDisplay_.text ='x' + scores.powerup;
    this.scoreBtns_.total.textDisplay_.text = scores.total;
};

QuizResultUI.prototype.setResult_ = function(result){
	this.resultText_.text = result > 0 ? "YOU WIN!" : (result == 0 ? "YOU TIE!" : "YOU LOSE!");
};

QuizResultUI.settings = {
	resultText:{x: CONST.BG.WIDTH / 2 , y: CONST.BG.HEIGHT / 6, textAlign: "center", textBaseline: "middle" },
	avatarL: {x: (CONST.BG.WIDTH - CONST.QUIZ_RESULT.AVATAR_RADIUS * 4 - CONST.QUIZ_RESULT.AVATAR_SPACE) / 2 +CONST.QUIZ_RESULT.AVATAR_RADIUS , y: CONST.QUIZ_RESULT.AVATAR_POSITION_Y},
	avatarR: {x: CONST.BG.WIDTH -((CONST.BG.WIDTH - CONST.QUIZ_RESULT.AVATAR_RADIUS * 4 - CONST.QUIZ_RESULT.AVATAR_SPACE) / 2 + CONST.QUIZ_RESULT.AVATAR_RADIUS), y: CONST.QUIZ_RESULT.AVATAR_POSITION_Y},
	pk:{x: CONST.BG.WIDTH / 2 - CONST.QUIZ_RESULT.PK_WIDTH / 2, y:CONST.QUIZ_RESULT.AVATAR_POSITION_Y},
	homeBtn: {x: (CONST.BG.WIDTH - CONST.QUIZ_RESULT.OPERATE_BTN_WIDTH) / 6, y: CONST.QUIZ_RESULT.OPERATE_BTN_POSITION_Y },
	rematchBtn: {x: (CONST.BG.WIDTH - CONST.QUIZ_RESULT.OPERATE_BTN_WIDTH) * 5/ 6, y: CONST.QUIZ_RESULT.OPERATE_BTN_POSITION_Y }

};

QuizResultUI.prototype.initDownPanel_ = function (){
	var colors  = [
		["#8984D6","#A46EDE"],
		["#0E9CAA","#125083"],
		["#29A157","#00684D"],
		["#CBA945","#94730A"]
	];
	for(var i = 0; i< 4; i++){
		var g = new createjs.Graphics().beginLinearGradientFill(colors[i], [0, 0.5], CONST.BG.WIDTH /2, CONST.BG.HEIGHT - CONST.QUIZ_RESULT.DOWN_HEIGHT * (i+1), CONST.BG.WIDTH /2, CONST.BG.HEIGHT - CONST.QUIZ_RESULT.DOWN_HEIGHT * i).drawRect(0, CONST.BG.HEIGHT - CONST.QUIZ_RESULT.DOWN_HEIGHT * (i+1), CONST.BG.WIDTH, CONST.QUIZ_RESULT.DOWN_HEIGHT);
		var shape = new createjs.Shape(g); 
		this.addChild(shape);
	}
};

QuizResultUI.prototype.setAvatars = function(){
	//init avatar 
    this.avatarL_ = new quiz.features.game.v.component.Avatar(CONST.QUIZ_RESULT.AVATAR_RADIUS, '#478868', "", 0.75 ); 
    this.avatarL_.set(QuizResultUI.settings.avatarL);
    this.avatarR_ = new quiz.features.game.v.component.Avatar(CONST.QUIZ_RESULT.AVATAR_RADIUS , '#EF565B', "", 0.75); 
    this.avatarR_.set(QuizResultUI.settings.avatarR);

    this.addChild(this.avatarL_, this.avatarR_);
};

QuizResultUI.prototype.setProfiles_ = function(){
	this.nameL_ = new createjs.Text("","35px " + CONST.FONT.ALL, "#FAFAFA"); 
	this.nameL_.set({x: QuizResultUI.settings.avatarL.x + CONST.QUIZ_RESULT.AVATAR_RADIUS , y: QuizResultUI.settings.avatarL.y + 80, textAlign: "right"}); 
	this.countryL_ = new createjs.Text("","25px " + CONST.FONT.ALL, "#FAFAFA");
	this.countryL_.set({x: QuizResultUI.settings.avatarL.x + CONST.QUIZ_RESULT.AVATAR_RADIUS , y: QuizResultUI.settings.avatarL.y + 130, textAlign: "right"}); 

	this.nameR_ = new createjs.Text("","35px " + CONST.FONT.ALL, "#FAFAFA");
	this.nameR_.set({x: QuizResultUI.settings.avatarR.x - CONST.QUIZ_RESULT.AVATAR_RADIUS , y: QuizResultUI.settings.avatarR.y + 80, textAlign: "left"});   

	this.countryR_ = new createjs.Text("","25px " + CONST.FONT.ALL, "#FAFAFA");
	this.countryR_.set({x: QuizResultUI.settings.avatarR.x - CONST.QUIZ_RESULT.AVATAR_RADIUS , y: QuizResultUI.settings.avatarR.y + 130, textAlign: "left"}); 

	this.addChild(this.nameL_, this.countryL_,this.nameR_, this.countryR_);
};

QuizResultUI.prototype.setPlayerInfos = function(players){
	this.nameL_.text = players[0].name;
	this.countryL_.text = players[0].country;
	this.nameR_.text = players[1].name;
	this.countryR_.text = players[1].country;

	this.avatarL_.setAvatar(players[0].avatar);
	this.avatarR_.setAvatar(players[1].avatar);
};

