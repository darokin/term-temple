## TODO

 - [x] Dans WidgetANSI mettre juste un moduleANSI et dégager le code en doublon
 - [x] Dans screen.cpp dégager le WINDOW* !!!! pas de trace de curses mettre un moduleOneLiner en pied (voir si mieux de le gérer dans le widgetManager qui a déjà du 'dur' pour le appLauncher par exemple) YES MOVE STATUS TO WMGR!
 - [x] timeInMilliseconds() dans Utils() et tick() dans globals... ... ... OK c pas si fou au final...
 - [x] rename in globals OK sans status c plus clean
 - [x] voir TODO dans FileExplorer, globals::Keys::... DONE!
 - [x] rename mainDraw() as update() ?
 - [x] Dans widgetDialog avoir un constructeur sans filePath où on peut faire un addDialog manuel et charger sinon always le dialog du fichier dans le constructeur (constructor bypass with static initializer)
 - [ ] faire WidgetResizable ou WidgetScrollable entre WidgetTextFile ou WidgetFileExplorer et base class Widget ?
 - [ ] Pour les widget RESIZABLE, mémorisé un prevPos et un prevSize pour revenir dessus lors retour normal après FullScreen {F}
 - [ ] Dans AppLauncher mettre des callbacks mappées au appNames. (Voir functors!)
 - [ ] Dand widgetTextFile extraire la partie lect>ss>vector of lines et reuse dans widgetDialog. Look msgBox, heritance de widget et pas widgetTextFile pour widgetDialog!
 - [ ] Remplace bOpening etc. et mettre un state à OPENING / ACTIVE / CLOSING et faire anim closing
 - [ ] Régler les cas d'auto kill (q on widget that call delete on themselves removeWidget(.., true))
 - [ ] Debug mouse les clics semblent triggered de manière random on dirait...
 - [ ] Gérer le clic sur CROSS d'une fenêtre dans widgetManager et pas widget et remove trace de wmgr dans widget.cpp...
 - [ ] Plein de mémoire pas libérée (dans les spécif des widgets)
 - [ ] Catch2 test init widget + module + update pos + chekcpos