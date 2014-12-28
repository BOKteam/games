/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 上午11:37
 * Write the description in this section.
 */
goog.provide("noone.features.game.MainGameFeature");
goog.require("bok.framework.core.MVCFeature");

noone.features.game.MainGameFeature = MainGameFeature;
BOK.inherits(MainGameFeature, MVCFeature);
function MainGameFeature(stage) {
    MVCFeature.call(this);

    this.addMediator(new noone.features.game.v.GameViewMediator(stage));
    this.addMediator(new noone.features.game.v.MenuViewMediator(stage));
}
