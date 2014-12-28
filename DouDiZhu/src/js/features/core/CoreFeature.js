/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-7-9
 * Time: 上午11:49
 * Write the description in this section.
 */

goog.provide("doudizhu.features.core.CoreFeature");
goog.require("bok.framework.core.MVCFeature");

goog.require("doudizhu.features.core.CoreFeatureNotes");
goog.require("doudizhu.features.core.GamePlayMediator");
goog.require("doudizhu.features.core.MultiplayMediator");

doudizhu.features.core.CoreFeature = CoreFeature;
BOK.inherits(CoreFeature, MVCFeature);
/**@constructor
 * */
function CoreFeature() {
    MVCFeature.call(this);

    this.localGame_ = new doudizhu.features.core.GamePlayMediator();
    this.onlineGame_ = new doudizhu.features.core.MultiplayMediator();
}


CoreFeature.prototype.initLocalGame = function() {
    this.uninstall();

    this.mediators = [];
    this.addMediator(this.localGame_);

    this.setup();
};

CoreFeature.prototype.initOnlineGame = function() {
    this.uninstall();

    this.mediators = [];
    this.addMediator(this.onlineGame_);

    this.setup();
};