/**
 * Created by Envee.
 *
 * Date: 14-11-17
 * Time: pm 13:28
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("quiz.features.game.v.MainViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("org.createjs.easeljs.EaselJS");
goog.requireAll("quiz.features.game.v.component.*");


quiz.features.game.v.MainViewMediator = MainViewMediator;
BOK.inherits(MainViewMediator, BaseMediator);

function MainViewMediator(stage) {
	BaseMediator.call(this);
	this.ui_ = new quiz.features.game.v.component.MainUI();
	stage.addChild(this.ui_);
	this.stage_ = stage;
	this.ui_.addEventListener('click', Delegate.create(this, this.onTabClick_));
}

MainViewMediator.prototype.onTabClick_ = function(){
	this.ui_.hide(Delegate.create(this,function(){
		this.stage_.removeChild(this.ui_);
		this.sendNotification(MainGameFeatureNotes.getInternalNote('GO_TOPIC'));	
	}));
	
};