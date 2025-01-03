# Projekt Status

Praćenje potrebnih i gotovih značajki projekta.

### Todo

- [ ] Napravi da custom ECF terminal učitava dinamički vrijednosti iz JSON roadnet filea
- [ ] Napravi custom ECF terminal
    - zapravo array već postojećih vrijednosti (X,Y...)
- [ ] Odvojiti engine od klase evaluatora
    - ukloniti stvaranje instance enginea u samom evaluatoru
- [ ] Riješiti se hardkodova vezano za topologiju primjera
    - trenutno su svi semafori/ceste hardkodirani u evaluatoru, umjesto da budu dinamički učitani iz JSON-a mape

### Gotovo ✓

- [X] Nađi i isprobaj neku kompleksniju topologiju od demo mape
- [x] Dodati "žuto svijetlo"
    - mogućnost konfiguracije u config.xml hoće li postojati ili ne (te koliko će trajati ako da)
- [x] Normalizacija fitnessa
    - trenutno je minimum fitnessa **time(auti_postoje_na_mapi) - time(auti_cekaju_na_semaforu)**
    - bolje bi bilo da je minimum 0 odnosno da se broji samo **time(auti_cekaju_na_semaforu)**
- [x] Napraviti evaluator prometa
    - fitness => prosječno vrijeme postojanja svih vozila od početka do kraja simulacije
- [x] Srediti CMakeLists.txt
- [x] Implementirati ECF kao library za algoritam učenja agenta
- [x] Implementirati CityFlow kao simulator prometa