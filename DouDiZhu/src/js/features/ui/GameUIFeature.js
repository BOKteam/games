/**
 * Created by lydashsbok on 14-7-16.
 */
goog.provide("doudizhu.features.ui.GameUIFeature");
goog.require("bok.framework.core.MVCFeature");
goog.require("doudizhu.features.ui.GameUIFeatureNotes");
goog.require("doudizhu.features.ui.v.MenuMediator");
goog.require("doudizhu.features.ui.v.GameUIMediator");



doudizhu.features.ui.GameUIFeature = GameUIFeature;

BOK.inherits(GameUIFeature, MVCFeature);
function GameUIFeature(stage) {
    MVCFeature.call(this);

    this.addMediator(new doudizhu.features.ui.v.GameUIMediator(stage));
    this.addMediator(new doudizhu.features.ui.v.MenuMediator(stage));
}

