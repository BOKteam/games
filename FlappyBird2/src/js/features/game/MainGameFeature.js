/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 上午11:37
 * Write the description in this section.
 */
goog.provide("app.features.game.MainGameFeature");
goog.require("bok.framework.core.MVCFeature");
goog.require("app.features.game.v.GameViewMediator");

app.features.game.MainGameFeature = MainGameFeature;
BOK.inherits(MainGameFeature, MVCFeature);
function MainGameFeature(stage) {
    MVCFeature.call(this);

    this.addMediator(new app.features.game.v.GameViewMediator(stage));
}