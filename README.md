# Projet réalisé dans le cadre du module "IA - Jeux"

Ce projet est constitué de deux parties :

1. Résolution des problèmes Nqueens et Knight
2. Résolution du jeu du taquin

> Le but ici est d'illustrer certains algorithmes de parcourt ainsi que l'utilisation d'arbres de recherche à travers de ces jeux (échecs, jeu du taquin)

---

- [Projet réalisé dans le cadre du module "IA - Jeux"](#projet-réalisé-dans-le-cadre-du-module-ia---jeux)
  - [****Jeu du Taquin***](#jeu-du-taquin)
  - [Principe](#principe)
    - [Nous avons implémenté les algorithmes suivants pour ce problème](#nous-avons-implémenté-les-algorithmes-suivants-pour-ce-problème)
    - [Structures de données](#structures-de-données)
    - [Structure du code](#structure-du-code)
    - [Compraison des algorithmes **(UCS vs A\*)**](#compraison-des-algorithmes-ucs-vs-a)
  - [Voir aussi : ***N-Queens et Déplacement Cavalier***](#voir-aussi--n-queens-et-déplacement-cavalier)

---

## [****Jeu du Taquin***](https://github.com/nathan-barbier/Nqueens-Knights)

![Wikipedia - 8Queens Minconflict](https://upload.wikimedia.org/wikipedia/commons/thumb/9/92/Great_presidential_puzzle2.jpg/799px-Great_presidential_puzzle2.jpg)

[Wikipedia - 15 Puzzle (Jeu du Taquin)](https://fr.wikipedia.org/wiki/Taquin)

## Principe

> Le but de problème est de trouver un ensemble de déplacements des cases du jeu afin de les ranger dans l'ordre croissant, en laissant une case vide dans le coin en bas à droite.

### Nous avons implémenté les algorithmes suivants pour ce problème

- **UCS** (*Uniform Cost Search*)
- **A*** (*A Star*)

### Structures de données

- On utilise un arbre de recherche qui contient les différents états de notre plateau de jeu
- On utilise deux listes chaînées pour faire tourner les algorithmes rappelés plus haut
  
### Structure du code

- La structure des noeuds est gérée dans le fichier **item.h**.
- Toutes les fonctions qui permettent de créer, modifier ou rechercher à l'interieur des listes sont dans le module **list.c**.
- L'initialisation, la vérification et la création de nouveaux états du plateau de jeu sont gérées dans le module **board.c**.
- Les algorithmes sont implémentés dans le module **taquin.c**.

### Compraison des algorithmes **(UCS vs A\*)**

|    UCS    | Etapes | Noeuds parcourus | Temps              | |    A*     | Etapes | Noeuds parcourus | Temps              |
| :-------: | :----: | :--------------: | :----------------: |-| :-------: | :----: | :--------------: | :----------------: |
| Facile    |   12   |      1834        | ~10<sup>-1</sup> s | | Facile    |   12   |       109        | <10<sup>-3</sup> s |
| Moyen     |   26   |     161108       | ~10<sup>3</sup> s  | | Moyen     |   26   |      36801       | ~10<sup>2</sup> s  |
| Difficile |   29   |     181065       | <10<sup>4</sup> s  | | Difficile |   29   |      84593       | <10<sup>3</sup> s  |
  
> Dans ce cas l'algorithme **A\*** est nettement plus rapide.

---

## Voir aussi : [***N-Queens et Déplacement Cavalier***](https://github.com/nathan-barbier/Nqueens-Knights)
