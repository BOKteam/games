/**
 * Created by Envee.
 *
 * Date: 14-10-10
 * Time: 下午4:48
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("noone.features.game.v.MenuViewMediator");
goog.require("noone.features.game.v.component.MainMenu");


noone.features.game.v.MenuViewMediator = MenuViewMediator;
BOK.inherits(MenuViewMediator, BaseMediator);
function MenuViewMediator(stage) {
    BaseMediator.call(this);
    //stage.stage.enableMouseOver(1);
    this.menu_ = new noone.features.game.v.component.MainMenu();
    stage.addChild(this.menu_);

    this.menu_.menuStart_.addEventListener('click', Delegate.create(this, this.onMenuStartClick_));
}

MenuViewMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(MainGameFeatureNotes.getOutputNoteWithFeatureName('BACK_MENU'), this.backMenu_, BaseMediator.SCOPE.PARENT);
    MenuViewMediator.superClass_.declareInterestedNotifications.call(this);
};

MenuViewMediator.prototype.onMenuStartClick_ = function(){
    this.menu_.hide();
    this.sendParentNotification(MainGameFeatureNotes.getOutputNoteWithFeatureName('START_GAME'));
};

MenuViewMediator.prototype.backMenu_ = function(){
    this.menu_.show();
};