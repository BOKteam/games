/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午9:06
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("tzfe.features.game.MainGameFeature");
goog.require("bok.framework.core.MVCFeature");
goog.require("tzfe.features.game.v.GameViewMediator");

tzfe.features.game.MainGameFeature = MainGameFeature;
BOK.inherits(MainGameFeature, MVCFeature);
function MainGameFeature(stage) {
    MVCFeature.call(this);

    this.addMediator(new tzfe.features.game.v.GameViewMediator(stage));
}
