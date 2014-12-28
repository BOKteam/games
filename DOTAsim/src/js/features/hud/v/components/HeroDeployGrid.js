/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 10/03/14
 * Time: 10:12
 * To change this template use File | Settings | File Templates.
 */

goog.provide("dotasim.features.hud.v.components.HeroDeployGrid");
goog.require("org.createjs.easeljs.EaselJS");

goog.require("dotasim.features.hud.v.components.HeroIcon");
/**
 * @param {Object} settings
 * @param {number} lane
 * @constructor
 * */
dotasim.features.hud.v.components.HeroDeployGrid = function(settings, lane)
{
    createjs.Container.call(this);

    /** @public */
    this.lane = lane;

    this.deployedHeroes_ = [];
    this.settings_ = settings;

    this.bg_ = new createjs.Shape();
    this.bg_.graphics.beginFill('rgba(0,0,0,0.01)').drawRect(0, 0, this.settings_.WIDTH, this.settings_.HEIGHT);
    this.addChild(this.bg_);

    this.grid_ = new createjs.Shape();
    this.addChild(this.grid_);
    this.drawGrid_('#000000');

    this.addEventListener(Event.CLICK, Delegate.create(this, this.onClick_));
};
BOK.inherits(dotasim.features.hud.v.components.HeroDeployGrid, createjs.Container);

/**
 * @param {dotasim.features.hud.v.components.HeroIcon} heroIcon
 * @return {boolean} return true if hero successfully deployed on lane.
 * */
dotasim.features.hud.v.components.HeroDeployGrid.prototype.deployHero = function(heroIcon)
{
    if(this.deployedHeroes_.length >= dotasim.GamePlayData.GENERAL.LANE_MAX_HERO)
        return false;

    this.deployedHeroes_.push(heroIcon);
    heroIcon.deployed();
    heroIcon.addEventListener('retract_finished', Delegate.create(this, this.onHeroRetracted_));
    heroIcon.x = this.settings_.WIDTH / 2;
    heroIcon.y = this.settings_.HEIGHT / 2;
    this.addChild(heroIcon);
    this.positionIcons_();

    return true;
};
dotasim.features.hud.v.components.HeroDeployGrid.prototype.disable = function()
{
    this.drawGrid_('#000000');
};

dotasim.features.hud.v.components.HeroDeployGrid.prototype.enable = function()
{
    if(this.deployedHeroes_.length >= dotasim.GamePlayData.GENERAL.LANE_MAX_HERO)
        this.drawGrid_('#990000');
    else
        this.drawGrid_('#009900');
};

/**
 * @private
 * */
dotasim.features.hud.v.components.HeroDeployGrid.prototype.onHeroRetracted_ = function(e)
{
    var retractedHero = e.target;

    BOK.findAndRemove(this.deployedHeroes_, retractedHero);
    this.removeChild(retractedHero);
    this.positionIcons_();
};
/**
 * @private
 * */
dotasim.features.hud.v.components.HeroDeployGrid.prototype.onClick_ = function()
{
    this.dispatchEvent(Event.SELECTED);
};
/**
 * @private
 * */
dotasim.features.hud.v.components.HeroDeployGrid.prototype.positionIcons_ = function()
{
    var offsetX = (this.settings_.WIDTH - this.settings_.BORDER_SIZE) / 2;
    var offsetY = (this.settings_.HEIGHT - this.settings_.BORDER_SIZE) / 2;
    BOK.each(this.deployedHeroes_, function(hero, index){
        var newPos = {
            x: (index % 2) *  offsetX + this.settings_.BORDER_SIZE,
            y: Math.floor(index / 2) * offsetY + this.settings_.BORDER_SIZE
        };
        if(newPos.x != hero.x || newPos.y != hero.y)
        {
            EaselAnimationHelper.moveTo(hero, newPos.x, newPos.y, 300);
        }
    }, this);
};
/**
 * @param {String} color
 * */
dotasim.features.hud.v.components.HeroDeployGrid.prototype.drawGrid_ = function(color)
{
    this.grid_.graphics.clear();
    this.grid_.graphics.beginFill(color).drawRect(0, 0, this.settings_.WIDTH, this.settings_.BORDER_SIZE);
    this.grid_.graphics.beginFill(color).drawRect(0, (this.settings_.HEIGHT - this.settings_.BORDER_SIZE) / 2, this.settings_.WIDTH, this.settings_.BORDER_SIZE);
    this.grid_.graphics.beginFill(color).drawRect(0, this.settings_.HEIGHT - this.settings_.BORDER_SIZE, this.settings_.WIDTH, this.settings_.BORDER_SIZE);
    this.grid_.graphics.beginFill(color).drawRect(0, 0, this.settings_.BORDER_SIZE, this.settings_.HEIGHT);
    this.grid_.graphics.beginFill(color).drawRect((this.settings_.WIDTH - this.settings_.BORDER_SIZE) / 2, 0, this.settings_.BORDER_SIZE, this.settings_.HEIGHT);
    this.grid_.graphics.beginFill(color).drawRect(this.settings_.WIDTH - this.settings_.BORDER_SIZE, 0, this.settings_.BORDER_SIZE, this.settings_.HEIGHT);
};
