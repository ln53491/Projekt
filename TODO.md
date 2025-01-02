# Projekt Status

Praćenje potrebnih i gotovih značajki projekta.

### Todo

- [ ] Dodati "žuto svijetlo"
    - mogućnost konfiguracije u config.xml hoće li postojati ili ne (te koliko će trajati ako da)
- [ ] Odvojiti engine od klase evaluatora
    - ukloniti stvaranje instance enginea u samom evaluatoru
- [ ] Normalizacija fitnessa
    - trenutno je minimum fitnessa **time(auti_postoje_na_mapi) - time(auti_cekaju_na_semaforu)**
    - bolje bi bilo da je minimum 0 odnosno da se broji samo **time(auti_cekaju_na_semaforu)**

### Gotovo ✓

- [x] Napraviti evaluator prometa
    - fitness => prosječno vrijeme postojanja svih vozila od početka do kraja simulacije
- [x] Srediti CMakeLists.txt
- [x] Implementirati ECF kao library za algoritam učenja agenta
- [x] Implementirati CityFlow kao simulator prometa