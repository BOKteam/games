/**
 * Created by Envee.
 *
 * Date: 14-10-28
 * Time: pm 16:27
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("quiz.features.game.v.QuizResultViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("org.createjs.easeljs.EaselJS");
goog.requireAll("quiz.features.game.v.component.*");
goog.require("quiz.features.game.MainGameFeatureNotes");


quiz.features.game.v.QuizResultViewMediator = QuizResultViewMediator;
BOK.inherits(QuizResultViewMediator, BaseMediator);

function QuizResultViewMediator(stage) {
	BaseMediator.call(this);
	
	this.ui_ = new quiz.features.game.v.component.QuizResultUI();
	this.ui_.hide();
	stage.addChild(this.ui_);

	this.ui_.homeBtn_.addEventListener('click', Delegate.create(this, this.onHomeBtnClick_));
	this.ui_.rematchBtn_.addEventListener('click', Delegate.create(this, this.onRematchBtnClick_));
}

QuizResultViewMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(MainGameFeatureNotes.getInternalNote('FINISH_GAME'), this.onFinishGame_);
    QuizResultViewMediator.superClass_.declareInterestedNotifications.call(this);
};

QuizResultViewMediator.prototype.onHomeBtnClick_ = function(){
	this.ui_.hide();
	this.sendNotification(MainGameFeatureNotes.getInternalNote('PLAYER_LEFT_GAME'));
};
QuizResultViewMediator.prototype.onRematchBtnClick_ = function(){
	this.ui_.hide();
	this.sendNotification(MainGameFeatureNotes.getInternalNote('GO_REMATCH'));
};

QuizResultViewMediator.prototype.onFinishGame_ = function(msg){
	var param = msg.body;
	this.ui_.init(param.grade , param.playerSides, param.result);
	this.ui_.show();
};
