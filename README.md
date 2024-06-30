# BabyPasBros

Dans "BabyPasBros", le joueur vient en aide à un bébé en l'aidant à accomplir la quête ultime de ses rêves, 
dans ses rêves: récupérer sa tétine perdue! Pour cela, il devra naviguer le monde des nuages, l'explorer et surmonter
les obstacles qui s'y trouvent...

#	Générer le code

Pour générer le code du jeu, suivez la procédure cmake ci dessous :
http://www-inf.telecom-sudparis.eu/COURS/CSC4526/new_site/Supports/Documents/OutilsCSC4526/outilsCSC4526.html#cmake

# Gameplay

Déplacez vous à l'aide de votre clavier (ZQSD) et sauter en pressant la barre d'espace. Trouvez la tétine dans le niveau actuel pour passer au niveau d'après. Des bulles colorées flottent et rebondissent sur les rebords de votre écran : cliquez sur l'une d'entre elles pour y être attiré. Attention! Chaque bulle cliquée disparaîtra jusqu'à la fin de la partie. 

# Le code du jeu

Les informations des niveaux se trouvent dans les fichiers 'monde1.xml' et 'monde2.xml'. Il faut choisir lequel des deux utiliser dans la classe 'Main.cpp' et les niveaux dans le fichier seront chargés au lancement du jeu. \
Le diagramme de classe fournit avec le dossier source du projet permet de mieux comprendre les classes qui s'y trouvent. Dans ce diagramme de classe, toutes les méthodes virtuelles de 'Entity' sont décrites dans sa classe mais ne ré-apparaissent dans ses classes enfants que lorsqu'elles finalement sont implémentées.

# Idées d'amélioration du jeu et hypothèses d'implémentation

Ce sont ici des idées pour ajouter de nouvelles fonctionnalités au jeu, qui ont été pensées mais qui n'ont pas eu le temps d'être mises en place concrètement.

## Implémentation d'ennemis dans les niveaux

Dans classe Entity: 
* ajouter attribut bool Hit{false}
* ajouter méthode bool Hits() {return &Hit}
* ajouter méthode virtuelle bool isHit(bool &trueOrFalse)

La classe UnmovingEntity renvoie false par défaut pour isHit(...).

Ajouter une classe Monstre dérivée de Entity: \
Monstre::collide(...) met alors Monstre::Hit à true quand le joueur est considéré touché par un monstre ou son attaque et par défaut, Monstre::Hit est remis à false à la fin de la méthode Monstre::update(...).

Dans la classe Player, implémenter Player::isHit(bool &trueOrFalse) pour faire une action lorsque trueOrFalse = true donc le joueur est touché.

Dans la méthode Group::isHit(…), Group::mainCharacter appelle alors isHit(bool child->Hits()) où child est le pointeur de l'itérateur 'auto child : children' qui parcourt 'children' (vecteur contenant toutes les Entity du niveau).

## Implémentation d'un système de clés

Actuellement, les portes dans le jeu s'ouvrent sans condition sur demande du joueur. Ces portes, lorsqu'elles sont fermées, bloquent le passage au joueur. S'il veut passer à travers, il doit les ouvrir.

L'idée ici est alors de nécéssiter l'usage d'une clé (à trouver dans le niveau) chaque fois que le joueur veut ouvrir une porte.

La difficulté ici est que les clés doivent donc intéragir avec deux classes assez différentes: Door (Entity <|-- UnmovingEntity <|-- InteractiveUnmoving <|-- Door) et Player (Entity <|-- Player). Le joueur lorsqu'il trouve une clé doit pouvoir la ramasser s'il est suffisament proche. Il faut ensuite noter que le joueur a en sa possession une clé supplémentaire. Lorsqu'il s'approche d'une porte, s'il n'a pas de clé, il ne doit pas pouvoir l'ouvrir et s'il en a au moins une, en ouvrant la porte le joueur doit perdre une de ses clés.

Idée de réalisation :

Ajouter un enum class Object {None, Key} (plus tard, cet énum pourra être complété avec d'autres objets ramassables ou différents types de clés)

Dans Entity, ajouter :
* une méthode Object lootType() {return Object::None}
* une méthode Object needs() {return Object::None}
* un attribut bool isUsed{false} avec une méthode isUSed() {return &isUsed}
* un attribut bool isPickedUp{false} avec une méthode isPickedUp() {return &isPickedUp}

Ajouter une classe Cle dérivée de InteractiveUnmoving (car celle-ci avant d'être ramassée se comportera comme une tétine (classe Pacifier --|> InteractiveUnmoving) c'est à dire immobile dans le niveau et à l'approche du bébé, un texte apparaît pour indiquer qu'il peut la ramasser) avec:
* une méthode lootType() {return Object::Key} override \
et ajouter toutes les instances de Cle au fichier 'monde.xml' puis modifier le constructeur de Group pour que toutes les clés soient ajoutées dans Group::children.

Ajouter à la classe Door:
* une méthode Object Door::needs() {return enum Object::Key} override
* un attribut bool isUnlocked{false}
* une méthode unlocks() {if (isNear && !isUnlocked){isUnlocked = true} return isUnlocked}.

Dans la classe Group, méthode Group::handlePlayerEvent(), implémenter deux itérateurs imbriqués: \
'for (auto obj1:children){ \
|   for (auto obj2:children) {...} \
}' \
et dans '...', appliquer les conditions et méthodes suivantes : \
si ( (obj->needs() == obj2) -> lootType() && !(obj2 -> isUsed()) && (obj2 -> isFound()) ) alors \
|   si (obj->unlocks()) alors \
|   |   obj2 -> isUsed() et obj -> handlePlayerEvent()

L'intéraction du joueur qui ramasse la clé se fait similairement à celle avec la tétine, et le joueur met à ce moment Cle::isPickedUp à true.





