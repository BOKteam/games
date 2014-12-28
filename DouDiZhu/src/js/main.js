/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 04/10/13
 * Time: 16:18
 * To change this template use File | Settings | File Templates.
 */

//normally no need to change this root name space
goog.provide("root.main");

goog.require("bok.easelui.Stretcher");
goog.require("bok.apps.preloader.CanvasPreloaderApp");
goog.require("bok.apps.preloader.components.BlueCanvasSkin");
goog.require("doudizhu.DouDiZhuApp");
goog.require("doudizhu.AssetsList");

var preloaderApp, game, rootStretcher, imgContainer = {}, hudAPI  = window.parent && window.parent.hudAPI;
window.addEventListener('load', start);

function start() {
    rootStretcher = new Stretcher(document.getElementById('canvas'));
    var loadingBG = new createjs.Bitmap('assets/img/LoadingImage_C.png');
    var loadCanvasSkin = new BlueCanvasSkin(loadingBG);
    preloaderApp = new CanvasPreloaderApp(imgList, imgContainer, rootStretcher, loadCanvasSkin);
    preloaderApp.setLoadInterval(20);
    preloaderApp.addEventListener(Event.COMPLETE, assetsLoaded);

    preloaderApp.start();
}

function assetsLoaded() {
    //rootStretcher.removeChild(loadingBG);
    game = new DouDiZhuApp(rootStretcher);
    game.start();
}

