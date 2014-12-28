/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 21/02/14
 * Time: 20:38
 * To change this template use File | Settings | File Templates.
 */

goog.provide("dotasim.features.hud.v.components.GameHud");
goog.require("bok.easelui.EsPanel");
goog.require("bok.easelui.SosaIcon");

goog.require("dotasim.features.hud.v.components.HeroDeployGrid");
goog.require("dotasim.features.hud.v.components.CenterPanel");
goog.require("dotasim.features.hud.v.components.SidePanel");

dotasim.features.hud.v.components.GameHud = function()
{
    EsUIComponent.call(this);

    this.setting = dotasim.GameSettings.GameHud;

    /** @type {dotasim.features.hud.v.components.SidePanel}*/
    this.sidePanel_ = null;
    /** @type {dotasim.features.hud.v.components.CenterPanel}*/
    this.centerPanel_ = null;

    //init view
    this.createSidePanel_();
    this.createCenterPanel_();

    //hide hud to begin with
    this.hide();
};
BOK.inherits(dotasim.features.hud.v.components.GameHud, EsUIComponent);

dotasim.features.hud.v.components.GameHud.prototype.hide = function()
{
    dotasim.features.hud.v.components.GameHud.superClass_.hide.call(this);
    this.animationStarted_();
};

dotasim.features.hud.v.components.GameHud.prototype.show = function()
{
    dotasim.features.hud.v.components.GameHud.superClass_.show.call(this);
    this.animationStarted_();

    this.dispatchEvent('open');
};

/**
 * @param {Array} heroes
 * */
dotasim.features.hud.v.components.GameHud.prototype.setHeroData = function(heroes)
{
    BOK.each(heroes, function(hero){
        this.centerPanel_.addHero(hero);
    }, this);
};

/**
 * @param {Object} data
 * */
dotasim.features.hud.v.components.GameHud.prototype.setPlayerData = function(data)
{
    this.centerPanel_.setPlayerData(data);
};


/**
 * @private
 * */
dotasim.features.hud.v.components.GameHud.prototype.animationStarted_ = function()
{
    this.animationInProgress_ = true;
    setTimeout(Delegate.create(this, this.animationFinished_), dotasim.GameSettings.GameHud.SHOW_HIDE_ANIMATION_TIME);
};
/**
 * @private
 * */
dotasim.features.hud.v.components.GameHud.prototype.animationFinished_ = function()
{
    this.animationInProgress_ = false;
};
/** @private
 * */
dotasim.features.hud.v.components.GameHud.prototype.createSidePanel_ = function()
{
    this.sidePanel_ = new dotasim.features.hud.v.components.SidePanel();
    this.sidePanel_.addEventListener('hero_retracted', Delegate.create(this, this.onHeroRetracted_));
    this.sidePanel_.addEventListener('hero_deployed', Delegate.create(this, this.onHeroDeployed_));
    this.sidePanel_.hudToggleButton.addEventListener(Event.CLICK, Delegate.create(this, this.onToggleClicked_));
    this.addChild(this.sidePanel_);
    this.link(this.sidePanel_);
};
/** @private
 * */
dotasim.features.hud.v.components.GameHud.prototype.createCenterPanel_ = function()
{
    this.centerPanel_ = new dotasim.features.hud.v.components.CenterPanel();
    this.centerPanel_.addEventListener(Event.SELECTED, Delegate.create(this, this.onCenterPanelHeroSelected_));
    this.centerPanel_.addEventListener(Event.UNSELECTED, Delegate.create(this, this.onCenterPanelHeroUnselected_));
    this.centerPanel_.mutuallyExclude(this.sidePanel_.heroLibButton);
    this.addChild(this.centerPanel_);
    this.link(this.centerPanel_);
};
/** @private
 * */
dotasim.features.hud.v.components.GameHud.prototype.onHeroRetracted_ = function(e)
{
    /** @type {dotasim.features.hud.v.components.HeroIcon}*/
    var heroIcon = e.hero;

    this.centerPanel_.regainHeroIcon(heroIcon);
    this.dispatchEvent({type:'hero_retracted', id:heroIcon.id});
};
/** @private
 * */
dotasim.features.hud.v.components.GameHud.prototype.onHeroDeployed_ = function(e)
{
    /** @type {dotasim.features.hud.v.components.HeroIcon}*/
    var heroIcon = e.hero;
    /** @type {number}*/
    var heroLane = e.lane;

    this.centerPanel_.releaseHeroIcon(heroIcon);

    this.dispatchEvent({type:'hero_deployed', id:heroIcon.id, lane:heroLane});
};
/** @private
 * */
dotasim.features.hud.v.components.GameHud.prototype.onCenterPanelHeroUnselected_ = function()
{
    this.sidePanel_.disableDeployment();
};
/** @private
 * */
dotasim.features.hud.v.components.GameHud.prototype.onCenterPanelHeroSelected_ = function(e)
{
    this.sidePanel_.enableDeployment(e.hero);
};
/** @private
 * */
dotasim.features.hud.v.components.GameHud.prototype.onToggleClicked_ = function()
{
    if(this.animationInProgress_)
        return;

    if(this.isHide())
        this.show();
    else
        this.hide();
};
