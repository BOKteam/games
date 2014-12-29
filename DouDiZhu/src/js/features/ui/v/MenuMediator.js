/**
 * Created by lys.
 * User: Liu Xinyi
 * Date: 14-7-28
 * Time: 上午9:31
 * Write the description in this section.
 */

goog.provide("doudizhu.features.ui.v.MenuMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("doudizhu.features.ui.v.components.MainMenu");

doudizhu.features.ui.v.MenuMediator = MenuMediator;

BOK.inherits(MenuMediator, BaseMediator);
function MenuMediator(stage) {
    BaseMediator.call(this);

    this.menu_ = new doudizhu.features.ui.v.components.MainMenu();
    stage.addChild(this.menu_);

    this.menu_.multi_.addEventListener('click', Delegate.create(this, this.onMenuMultiClick_));
    this.menu_.menuStart_.addEventListener('click', Delegate.create(this, this.onMenuStartClick_));
    this.menu_.menuHelp_.addEventListener('click', Delegate.create(this, this.onMenuHelpClick_));

    this.actionTimeoutId_ = null;
}

MenuMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(GameUIFeatureNotes.getOutputNote('BACK_TO_MENU'), this.onGameBack_);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('GAME_INIT'), this.onGameInit_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('NETWORK_WAITING'), this.onNetworkWaiting_, BaseMediator.SCOPE.PARENT);

    MenuMediator.superClass_.declareInterestedNotifications.call(this);
};

MenuMediator.prototype.onGameInit_ = function(){
    this.stopReadyActionTimeout_();
    this.menu_.hide();
};

MenuMediator.prototype.onGameBack_ = function(){
    this.menu_.show();
};


//************************//
//event listeners
MenuMediator.prototype.onMenuMultiClick_ = function(){
    if(this.menu_.blocked)
        return;

    this.menu_.showNetworkWaiting();
    this._app_.initOnlineGame();
    this.sendParentNotification(CoreFeatureNotes. getInputNoteWithFeatureName('READY_TO_START'));
};

MenuMediator.prototype.onMenuStartClick_ = function(){
    if(this.menu_.blocked)
        return;

    this._app_.initLocalGame();
    this.sendParentNotification(CoreFeatureNotes.getInputNoteWithFeatureName('READY_TO_START'));
};

MenuMediator.prototype.onMenuHelpClick_ = function(){
};

MenuMediator.prototype.onNetworkWaiting_ = function(msg) {
    this.menu_.setWaitingNum(msg.body.waitingNum);
    if(msg.body.readyDuration != null && this.actionTimeoutId_ == null){
        this.menu_.setReadydDuration(msg.body.readyDuration);
        this.stopReadyActionTimeout_();
        this.durationNum_ =  Math.round(msg.body.readyDuration / 1000);
        this.readyActionTimeout_();
    }
};

/**
 * player ready timeout start
 **/
MenuMediator.prototype.readyActionTimeout_ = function(){
    if(this.durationNum_ <= 0){
        this.stopReadyActionTimeout_();
        return;
    }
    this.menu_.setReadydDuration(this.durationNum_);
    this.durationNum_ --;
    this.actionTimeoutId_ = setTimeout(Delegate.create(this, this.readyActionTimeout_),1000);
};
/**
 * player ready timeout stop
 * */
MenuMediator.prototype.stopReadyActionTimeout_ = function() {
    if(this.actionTimeoutId_) {
        clearTimeout(this.actionTimeoutId_);
        this.actionTimeoutId_ = null;
    }
};
