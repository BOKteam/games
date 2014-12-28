/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.OptionPanel");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("quiz.features.game.v.component.OptionButton");

quiz.features.game.v.component.OptionPanel = OptionPanel;
BOK.inherits(OptionPanel, createjs.Container);

function OptionPanel(){
	createjs.Container.call(this);

    this.options_ = [];
    var optionWidth = (CONST.BG.WIDTH - CONST.PROGRESS_BAR.WIDTH * 2 - CONST.GAME_UI.PROGRESS_BAR_OFFSET * 2 - CONST.OPTION_PANEL.OPTION_SPACE * (CONST.OPTION_PANEL.OPTION_NUMBER + 1) ) / 2; 
    this.optionA_ = new quiz.features.game.v.component.OptionButton(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT, "0");

    this.optionB_ = new quiz.features.game.v.component.OptionButton(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT, "1");

    this.optionC_ = new quiz.features.game.v.component.OptionButton(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT, "2");

    this.optionD_ = new quiz.features.game.v.component.OptionButton(optionWidth, CONST.OPTION_PANEL.OPTION_HEIGHT, "3");

    this.options_.push(this.optionA_, this.optionB_, this.optionC_, this.optionD_);

    //this.addChild(this.optionA_, this.optionB_, this.optionC_, this.optionD_);
    this.hide();
}

OptionPanel.prototype.show = function(){
	this.visible =true;
};
OptionPanel.prototype.hide = function(){
	this.visible =false;
};

