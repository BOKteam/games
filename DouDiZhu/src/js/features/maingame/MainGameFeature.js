/**
 * Created by lys.
 * User: Liu Xinyi
 * Date: 14-7-9
 * Time: 上午11:49
 * Write the description in this section.
 */

goog.provide("doudizhu.features.maingame.MainGameFeature");
goog.require("bok.framework.core.MVCFeature");

goog.require("doudizhu.features.maingame.MainGameFeatureNotes");
goog.require("doudizhu.features.maingame.v.TableMediator");

doudizhu.features.maingame.MainGameFeature = MainGameFeature;
BOK.inherits(MainGameFeature, MVCFeature);
/**@param {createjs.Stage} stage
 * */
function MainGameFeature(stage) {
    MVCFeature.call(this);

    this.addMediator(new doudizhu.features.maingame.v.TableMediator(stage));
}
