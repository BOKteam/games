/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.OptionButton");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("bok.easelui.SosaIcon");
goog.require("bok.util.EaselAnimationHelper");

quiz.features.game.v.component.OptionButton = OptionButton;
BOK.inherits(OptionButton, createjs.Container);

/**
 * @param {number} width // the width of option button
 * @param {number} height // the height of option button
 * @param {string} option // the option of this
 */
function OptionButton(width, height, option){
	createjs.Container.call(this);
	this.option_ = option;
	this.statusBtn_ = new SosaIcon("", 40, {border: 0, fontColor: "#4FB087", bgColor: "rgba(255,255,255,0)"});
	this.addChild(this.statusBtn_);
	this.setOption(width, height);	
}

/**
 * change the option button text color
 * @param {string} color // the color string
 */
OptionButton.prototype.showAnswerStatus = function(isRight){
	this.statusBtn_.set({x: this.width_ * 3 / 4  , y: this.height_ / 2 - 20});
	if(isRight){
		this.text_.color = "#4FB087";
		this.statusBtn_.textDisplay_.text = '\u00e5';
		this.statusBtn_.textDisplay_.color = '#4FB087';
	}else{
		this.text_.color = "#B97175";
		this.statusBtn_.textDisplay_.text = '\u00E3';
		this.statusBtn_.textDisplay_.color = '#B97175';
	}
	this.statusBtn_.visible = true;
	EaselAnimationHelper.fadeIn(this.statusBtn_, 500);
		
};

OptionButton.prototype.setChosenColor = function(){
    this.text_.color = "#54A4FF";
};

OptionButton.prototype.clearAnswerStatus = function(){
	this.statusBtn_.visible = false;
};

OptionButton.prototype.setChoice = function(choice){
	this.text_.text = choice;
};

OptionButton.prototype.getOption = function(){
	return this.option_;
};


OptionButton.prototype.setOption = function(width, height){
	if(this.shape_){
		this.removeChild(this.shape_, this.text_);
	}
	this.width_ = width;
	this.height_ = height;
	this.text_ = new createjs.Text(this.option_, "25px " + CONST.FONT.ALL, "#1A1A1A");
	this.text_.set({x:width / 2 , y: height / 2, textAlign: "center", textBaseline:'middle', maxWidth: width});
	this.shape_ = new createjs.Shape(new createjs.Graphics().beginFill("#F8F8F8").drawRoundRect(0, 0, width, height, 4).endFill());
	this.addChildAt(this.text_,0);
	this.addChildAt(this.shape_,0);
	this.statusBtn_.visible = false;
};

