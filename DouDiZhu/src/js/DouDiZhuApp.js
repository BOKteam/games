/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 04/10/13
 * Time: 16:19
 * To change this template use File | Settings | File Templates.
 */
goog.provide("doudizhu.DouDiZhuApp");
goog.require("bok.framework.App");
goog.require("doudizhu.features.maingame.MainGameFeature");
goog.require("doudizhu.features.core.CoreFeature");
goog.require("doudizhu.features.ui.GameUIFeature");

/**
 * @param {createjs.Stage} stage
 * @constructor
 * */
function DouDiZhuApp(stage)
{
    App.call(this);

    //init assets & components

    //init mvc
    this.coreFeature_ = new doudizhu.features.core.CoreFeature();
    this.addFeature(this.coreFeature_);
    this.addFeature(new doudizhu.features.maingame.MainGameFeature(stage));
    this.addFeature(new doudizhu.features.ui.GameUIFeature(stage));
}
BOK.inherits(DouDiZhuApp, App);

DouDiZhuApp.prototype.initLocalGame = function() {
    this.coreFeature_.initLocalGame();
};

DouDiZhuApp.prototype.initOnlineGame = function() {
    this.coreFeature_.initOnlineGame();
};

