/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午8:55
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("root.main");

goog.require("bok.easelui.Stretcher");
goog.require("bok.apps.preloader.CanvasPreloaderApp");
goog.require("bok.apps.preloader.components.BlueCanvasSkin");
goog.require("tzfe.TzfeApp");
goog.require("tzfe.AssetsList");

var preloaderApp, game, rootStretcher, imgContainer = {};
window.addEventListener('load', start);

function start() {
    rootStretcher = new Stretcher(document.getElementById('canvas'),540, 960);
    rootStretcher.hideFPS();
    var loadingBG = new createjs.Bitmap('assets/img/LoadingImage_C.png');
    var loadCanvasSkin = new BlueCanvasSkin(loadingBG);
    preloaderApp = new CanvasPreloaderApp(imgList, imgContainer, rootStretcher, loadCanvasSkin);
    preloaderApp.setLoadInterval(20);
    preloaderApp.addEventListener(Event.COMPLETE, assetsLoaded);

    preloaderApp.start();
}

function assetsLoaded() {
    game = new TzfeApp(rootStretcher);
    game.start();
}
