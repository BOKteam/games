/**
 * @author lys.BOK
 * Date: 14-3-10
 * Time: 下午10:46
 *
 * File over view.
 */
goog.provide("dotasim.features.hud.v.components.SidePanel");
goog.require("bok.easelui.EsPanel");

goog.require("dotasim.features.hud.v.components.HeroIcon");

dotasim.features.hud.v.components.SidePanel = function()
{
    var settings = dotasim.GameSettings.GameHud.SidePanel;
    EsPanel.call(this, settings.WIDTH, settings.HEIGHT, {useDefaultCloseButton:false, useDefaultShowHideAnim:false});

    /** @public
     *  @type {SosaIcon}*/
    this.hudToggleButton = null;
    /** @public
     *  @type {SosaIcon}*/
    this.heroLibButton = null;
    /** @private
     *  @type {dotasim.features.hud.v.components.HeroIcon}*/
    this.selectedIcon_ = null;

    //Create hero deploy grid for 3 lanes
    this.deployGridAry_ = [];
    for(var i = 0; i < 3; i++)
    {
        this.deployGridAry_[i] = new dotasim.features.hud.v.components.HeroDeployGrid(settings.DeployGrid, i);
        this.deployGridAry_[i].y = i * (settings.DeployGrid.HEIGHT + settings.DeployGrid.GAP_Y);
        this.deployGridAry_[i].addEventListener(Event.SELECTED, Delegate.create(this, this.onGridSelected_));
        this.addChild(this.deployGridAry_[i]);
    }

    this.createButtons_(settings);

};
BOK.inherits(dotasim.features.hud.v.components.SidePanel, EsPanel);

/**
 * @overload
 * */
dotasim.features.hud.v.components.SidePanel.prototype.hide = function()
{
    var TIME = dotasim.GameSettings.GameHud.SHOW_HIDE_ANIMATION_TIME;

    createjs.Tween.get(this.buttonLayer_).to({alpha:0.1}, TIME, createjs.Ease.cubicOut);
    createjs.Tween.get(this).to({x:-this.width}, TIME, createjs.Ease.elasticOut);

    dotasim.features.hud.v.components.SidePanel.superClass_.hide.call(this);
};
/**
 * @overload
 * */
dotasim.features.hud.v.components.SidePanel.prototype.show = function()
{
    var TIME = dotasim.GameSettings.GameHud.SHOW_HIDE_ANIMATION_TIME;

    createjs.Tween.get(this.buttonLayer_).to({alpha:1}, TIME, createjs.Ease.cubicOut);
    createjs.Tween.get(this).to({x:0}, TIME, createjs.Ease.elasticOut);

    dotasim.features.hud.v.components.SidePanel.superClass_.show.call(this);
};

/**
 * @param {dotasim.features.hud.v.components.HeroIcon} heroIcon
 * */
dotasim.features.hud.v.components.SidePanel.prototype.enableDeployment = function(heroIcon)
{
    this.selectedIcon_ = heroIcon;
    BOK.each(this.deployGridAry_, function(grid){
        grid.enable();
    });
};

dotasim.features.hud.v.components.SidePanel.prototype.disableDeployment = function()
{
    this.selectedIcon_ = null;
    BOK.each(this.deployGridAry_, function(grid){
        grid.disable();
    });
};

/**
 * @private
 * */
dotasim.features.hud.v.components.SidePanel.prototype.onHeroRetract_ = function(e)
{
    e.target.removeAllEventListeners('retract_finished');
    this.dispatchEvent({type: 'hero_retracted', hero:e.target});
};
/**
 * @private
 * */
dotasim.features.hud.v.components.SidePanel.prototype.onGridSelected_ = function(e)
{
    if(this.selectedIcon_)
    {
        /**@type {dotasim.features.hud.v.components.HeroDeployGrid}*/
        var grid = e.target;
        var heroIcon = this.selectedIcon_;
        if(grid.deployHero(heroIcon))
        {
            heroIcon.addEventListener('retract_finished', Delegate.create(this, this.onHeroRetract_));
            this.dispatchEvent({type:'hero_deployed', hero:heroIcon, lane:grid.lane});
        }
    }
};


/**
 * @private
 * */
dotasim.features.hud.v.components.SidePanel.prototype.createButtons_ = function(settings)
{
    //add toggle button
    this.hudToggleButton = new SosaIcon(SosaIcon.ICON.GAME);
    this.hudToggleButton.y = -this.hudToggleButton.height;

    //add hero lib toggle button
    this.heroLibButton = new SosaIcon(SosaIcon.ICON.MAN3, 20, {width:40});
    this.heroLibButton.addEventListener(Event.CLICK, Delegate.create(this.heroLibButton, this.heroLibButton.hide));
    this.heroLibButton.show = function()
    {
        this.visible = true;
        this.y = -this.height;
        this.alpha = 0;
        createjs.Tween.get(this).to({y:0, alpha:1}, 500, createjs.Ease.bounceOut);
        SosaIcon.prototype.show.call(this);
    };
    this.heroLibButton.hide = function()
    {
        createjs.Tween.get(this).to({y:-this.height, alpha:0}, 500);
        SosaIcon.prototype.hide.call(this);
    };

    this.buttonLayer_ = new createjs.Container();
    this.buttonLayer_.x = settings.WIDTH;
    this.buttonLayer_.y = this.hudToggleButton.height;
    this.buttonLayer_.addChild(this.heroLibButton);
    this.buttonLayer_.addChild(this.hudToggleButton);
    this.addChild(this.buttonLayer_);
};
