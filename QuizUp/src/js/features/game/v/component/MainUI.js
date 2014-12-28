/**
 * Created by Envee.
 *
 * Date: 14-11-17
 * Time: pm 13:18
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide('quiz.features.game.v.component.MainUI');
goog.require('org.createjs.easeljs.EaselJS');
goog.require("bok.util.EaselAnimationHelper");
goog.require("quiz.features.game.v.component.PKIcon");

goog.require('quiz.AssetsList');

quiz.features.game.v.component.MainUI = MainUI;
BOK.inherits(MainUI, createjs.Container);

function MainUI (){
	createjs.Container.call(this);
	this.bg_ = new createjs.Bitmap("assets/img/QuziMate_main.png");
	this.addChild(this.bg_);
	
	this.tip_ = new createjs.Text("tap to start","35px " + CONST.FONT.ALL, "#FF5454");
	this.tip_.set(MainUI.settings.tip);
	this.tipAnimation_();
	this.addChild(this.tip_);
}
MainUI.settings = {
	tip: {x:  CONST.BG.WIDTH / 2, y: CONST.BG.HEIGHT * 9 / 10, textAlign:'center', textBaseline: 'top', alpha: 0.7}
};

MainUI.prototype.tipAnimation_ = function(){
	this.tip_.alpha = 0.7
	this.tickAnimation_ = createjs.Tween.get(this.tip_).to({alpha: 0.2},CONST.MAIN_UI.TIP_ANIMATE_TIME).call(Delegate.create(this, function(){
		createjs.Tween.get(this.tip_).to({alpha: 0.7},CONST.MAIN_UI.TIP_ANIMATE_TIME).call(Delegate.create(this, function(){
			this.tipAnimation_();
		}));	
	}));	
};

MainUI.prototype.show = function(){
	this.visible = true;
};
MainUI.prototype.hide = function(cb){
	this.alpha = 1;
	EaselAnimationHelper.disappear(this, CONST.TOPIC_UI.SWITCH_TIME).call(Delegate.create(this, function(){
		this.visible = false;
		this.tickAnimation_ = null;	
		this.tipAnimation_ = function(){return void(0);};
		cb();
	}));
	
};