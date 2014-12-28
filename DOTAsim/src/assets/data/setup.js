/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 18/02/14
 * Time: 14:34
 * To change this template use File | Settings | File Templates.
 */

goog.provide('dotasim.assets.data.setup');

goog.require('dotasim.assets.data.GameSettings');

/**
 * Unit data sample:
 *
{
    id: 'u001',
    type: 'BASE',
    party: 0,
    action:{type:'IDLE', target:null},
    speed:1,
    pos:{x:0, y:50},
    atkRange: 10,
    atk: 1,
    atkCD: 1000,
    atkCDCur: 0,
    hp: 10
}
 * */
dotasim.assets.data.setup.InitialUnitsData = (function(){
    var initUnits = [];
    var HQ = dotasim.model.HQ;
    var Tower = dotasim.model.Tower;
    var pos = BOK2DGeo.Vec2;
    var towerBaseY = 60;
    var towerGapY = 160;
    var towerBaseX1 = 120;
    var towerBaseX2 = 520;
    var towerGapX = 70;
    var posHQ0 = dotasim.GameSettings.POS.HQ['0'];
    var posHQ1 = dotasim.GameSettings.POS.HQ['1'];
    initUnits.push(new HQ(0, new pos(posHQ0.x, posHQ0.y)));
    initUnits.push(new Tower(0, new pos(posHQ0.x+6, posHQ0.y+6)));
    initUnits.push(new Tower(0, new pos(posHQ0.x+6, posHQ0.y-6)));
    initUnits.push(new Tower(0, new pos(towerBaseX1, towerBaseY)));
    initUnits.push(new Tower(0, new pos(towerBaseX1, towerBaseY+towerGapY)));
    initUnits.push(new Tower(0, new pos(towerBaseX1, towerBaseY+towerGapY*2)));
    initUnits.push(new Tower(0, new pos(towerBaseX1+towerGapX, towerBaseY)));
    initUnits.push(new Tower(0, new pos(towerBaseX1+towerGapX, towerBaseY+towerGapY)));
    initUnits.push(new Tower(0, new pos(towerBaseX1+towerGapX, towerBaseY+towerGapY*2)));
    initUnits.push(new Tower(0, new pos(towerBaseX1+towerGapX*2, towerBaseY)));
    initUnits.push(new Tower(0, new pos(towerBaseX1+towerGapX*2, towerBaseY+towerGapY)));
    initUnits.push(new Tower(0, new pos(towerBaseX1+towerGapX*2, towerBaseY+towerGapY*2)));
    initUnits.push(new HQ(1, new pos(posHQ1.x, posHQ1.y)));
    initUnits.push(new Tower(1, new pos(posHQ1.x-6, posHQ1.y+6)));
    initUnits.push(new Tower(1, new pos(posHQ1.x-6, posHQ1.y-6)));
    initUnits.push(new Tower(1, new pos(towerBaseX2, towerBaseY)));
    initUnits.push(new Tower(1, new pos(towerBaseX2, towerBaseY+towerGapY)));
    initUnits.push(new Tower(1, new pos(towerBaseX2, towerBaseY+towerGapY*2)));
    initUnits.push(new Tower(1, new pos(towerBaseX2-towerGapX, towerBaseY)));
    initUnits.push(new Tower(1, new pos(towerBaseX2-towerGapX, towerBaseY+towerGapY)));
    initUnits.push(new Tower(1, new pos(towerBaseX2-towerGapX, towerBaseY+towerGapY*2)));
    initUnits.push(new Tower(1, new pos(towerBaseX2-towerGapX*2, towerBaseY)));
    initUnits.push(new Tower(1, new pos(towerBaseX2-towerGapX*2, towerBaseY+towerGapY)));
    initUnits.push(new Tower(1, new pos(towerBaseX2-towerGapX*2, towerBaseY+towerGapY*2)));

    return initUnits;
})();