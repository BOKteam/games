/**
 * Created by Envee.
 *
 * Date: 14-11-07
 * Time: pm 16:17
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("quiz.features.game.v.MatchSuccessViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("org.createjs.easeljs.EaselJS");
goog.requireAll("quiz.features.game.v.component.*");
goog.require("quiz.features.game.MainGameFeatureNotes");


quiz.features.game.v.MatchSuccessViewMediator = MatchSuccessViewMediator;
BOK.inherits(MatchSuccessViewMediator, BaseMediator);

function MatchSuccessViewMediator(stage) {
	 BaseMediator.call(this);
	 this.ui_ = new quiz.features.game.v.component.MatchSuccessUI();
	 stage.addChild(this.ui_);
}

MatchSuccessViewMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GAME_INIT'), this.onGameInit_);
    MatchSuccessViewMediator.superClass_.declareInterestedNotifications.call(this);
};

MatchSuccessViewMediator.prototype.onGameInit_ = function(e){
	var playerSides = BOK.cloneObject(e.body.playerSides);
	var player = e.body.player;
    if(playerSides[0].name != player.name) {
        playerSides.push(playerSides.shift());
    }
    this.ui_.init(playerSides);
    this.ui_.show();
	setTimeout(Delegate.create(this, function(){
		this.ui_.hide();
		this.sendNotification(MainGameFeatureNotes.getInternalNote('GO_GAME'), e.body);
	}), 3000);
	
};