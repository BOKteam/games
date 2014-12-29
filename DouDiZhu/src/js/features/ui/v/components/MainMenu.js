
/**
 * Created by lys.
 * User: Liu Xinyi
 * Date: 14-7-28
 * Time: 上午9:26
 * Write the description in this section.
 */
goog.provide("doudizhu.features.ui.v.components.MainMenu");
goog.require("org.createjs.easeljs.EaselJS");

doudizhu.features.ui.v.components.MainMenu = MainMenu;
BOK.inherits(MainMenu, createjs.Container);

function MainMenu(){
    createjs.Container.call(this);

    this.blocked = false;

    this.menuBG_ = new createjs.Bitmap('assets/img/GameUI/bg01.png');
    this.multi_ = new createjs.Bitmap('assets/img/GameUI/multiplayer.png');
    this.menuStart_ = new createjs.Bitmap('assets/img/GameUI/start.png');
    this.menuHelp_ = new createjs.Bitmap('assets/img/GameUI/help.png');
    this.msgPanel_ = new createjs.Container();
    this.waitingNum_ = new createjs.Container();
    this.readyTime_ = new createjs.Container();

    //setup msg Panel
    var panel = new createjs.Bitmap('assets/img/GameUI/online_msg.png');
    panel.set({scaleX:1.5, scaleY:1.5});
    this.msgPanel_.addChild(panel);

    this.menuStart_.set({ x: 600, y: 250 });
    this.multi_.set({ x: 600, y: 330 });
    this.menuHelp_.set({ x: 600, y: 410 });
    this.msgPanel_.set({ visible:false, x: 200, y: 110 });
    this.readyTime_.set({ x: 50, y: 120 });
    this.waitingNum_.set({x: 250, y: 120 });

    this.msgPanel_.addChild(this.waitingNum_);
    this.msgPanel_.addChild(this.readyTime_);
    this.addChild(this.menuBG_);
    this.addChild(this.multi_);
    this.addChild(this.menuStart_);
    this.addChild(this.menuHelp_);
    this.addChild(this.msgPanel_);
}

MainMenu.prototype.showNetworkWaiting = function() {
    this.msgPanel_.visible = true;
    this.blocked = true;
};

MainMenu.prototype.hide = function() {
    this.visible = false;
    this.msgPanel_.visible = false;
    this.blocked = false;
};

MainMenu.prototype.show = function() {
    this.visible = true;
};

MainMenu.prototype.setReadydDuration = function(duration) {
    if (!this.readyTime_.text) {
        this.readyTime_.text = new createjs.Text("匹配剩余时间:" + duration, "18px Arial bold", "#FFFFFF");
        this.readyTime_.addChild(this.readyTime_.text);
    } else {
        this.readyTime_.text.text = "匹配剩余时间:" + duration;
    }
};
MainMenu.prototype.setWaitingNum = function(waitingNum) {
    if(!this.waitingNum_.text){
        this.waitingNum_.text = new createjs.Text("当前匹配人数:" + waitingNum, "18px Arial bold", "#FFFFFF");
        this.waitingNum_.addChild(this.waitingNum_.text);
    }else{
        this.waitingNum_.text.text = "当前匹配人数:" + waitingNum;
    }
};
