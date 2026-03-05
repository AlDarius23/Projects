BEGIN TRANSACTION;

DROP TABLE IF EXISTS Jucatori;
DROP TABLE IF EXISTS Intrebari;
DROP TABLE IF EXISTS Stare;
DROP TABLE IF EXISTS Sesiune;

CREATE TABLE Jucatori (ID INTEGER PRIMARY KEY AUTOINCREMENT, Nume TEXT UNIQUE, Scor INTEGER);

CREATE TABLE Intrebari (
    ID INTEGER PRIMARY KEY, 
    Text TEXT, 
    R1 TEXT, 
    R2 TEXT, 
    R3 TEXT, 
    Corect INTEGER
);

CREATE TABLE Stare (Cheie INTEGER PRIMARY KEY, CineRaspunde INTEGER, IntrebareCurenta INTEGER, JocInceput INTEGER);

CREATE TABLE Sesiune (
    Ord INTEGER PRIMARY KEY AUTOINCREMENT,
    IntrebareID INTEGER
);

INSERT INTO Stare VALUES (1, 1, 1, 0);

INSERT INTO Intrebari VALUES (1, 'Cine a castigat Cupa Mondiala 2022?', 'Franta', 'Argentina', 'Brazilia', 2);
INSERT INTO Intrebari VALUES (2, 'Cine are cele mai multe Baloane de Aur?', 'C. Ronaldo', 'Messi', 'Pele', 2);
INSERT INTO Intrebari VALUES (3, 'Echipa cu cele mai multe Ligi ale Campionilor?', 'AC Milan', 'Liverpool', 'Real Madrid', 3);
INSERT INTO Intrebari VALUES (4, 'Cine este "Regele" fotbalului?', 'Maradona', 'Pele', 'Hagi', 2);
INSERT INTO Intrebari VALUES (5, 'Ce echipa romaneasca a castigat CCE in 1986?', 'Dinamo', 'Steaua', 'Rapid', 2);
INSERT INTO Intrebari VALUES (6, 'Cate minute are o repriza?', '45', '90', '60', 1);
INSERT INTO Intrebari VALUES (7, 'Ce culoare are cartonasul de eliminare?', 'Galben', 'Albastru', 'Rosu', 3);
INSERT INTO Intrebari VALUES (8, 'In ce tara e stadionul Camp Nou?', 'Italia', 'Spania', 'Franta', 2);
INSERT INTO Intrebari VALUES (9, 'Cine a castigat prima Cupa Mondiala (1930)?', 'Uruguay', 'Brazilia', 'Italia', 1);
INSERT INTO Intrebari VALUES (10, 'Golgherul all-time din Champions League?', 'Messi', 'C. Ronaldo', 'Benzema', 2);

INSERT INTO Intrebari VALUES (11, 'Capitala Australiei?', 'Sydney', 'Melbourne', 'Canberra', 3);
INSERT INTO Intrebari VALUES (12, 'Capitala Turciei?', 'Istanbul', 'Ankara', 'Izmir', 2);
INSERT INTO Intrebari VALUES (13, 'Capitala Canadei?', 'Toronto', 'Ottawa', 'Vancouver', 2);
INSERT INTO Intrebari VALUES (14, 'Capitala Braziliei?', 'Rio de Janeiro', 'Brasilia', 'Sao Paulo', 2);
INSERT INTO Intrebari VALUES (15, 'Capitala Japoniei?', 'Tokyo', 'Kyoto', 'Osaka', 1);
INSERT INTO Intrebari VALUES (16, 'Capitala SUA?', 'New York', 'Washington DC', 'Los Angeles', 2);
INSERT INTO Intrebari VALUES (17, 'Capitala Egiptului?', 'Cairo', 'Alexandria', 'Luxor', 1);
INSERT INTO Intrebari VALUES (18, 'Capitala Italiei?', 'Milano', 'Venetia', 'Roma', 3);
INSERT INTO Intrebari VALUES (19, 'Capitala Germaniei?', 'Munchen', 'Berlin', 'Hamburg', 2);
INSERT INTO Intrebari VALUES (20, 'Capitala Spaniei?', 'Barcelona', 'Sevilla', 'Madrid', 3);

INSERT INTO Intrebari VALUES (21, 'Cand a inceput Al Doilea Razboi Mondial?', '1914', '1939', '1945', 2);
INSERT INTO Intrebari VALUES (22, 'Cine a fost primul imparat roman?', 'Cezar', 'Nero', 'Augustus', 3);
INSERT INTO Intrebari VALUES (23, 'Anul Revolutiei Franceze?', '1789', '1848', '1917', 1);
INSERT INTO Intrebari VALUES (24, 'Cine a descoperit America?', 'Magellan', 'Columb', 'Vasco da Gama', 2);
INSERT INTO Intrebari VALUES (25, 'Cand s-a scufundat Titanicul?', '1905', '1912', '1920', 2);
INSERT INTO Intrebari VALUES (26, 'Primul om pe Luna?', 'Yuri Gagarin', 'Buzz Aldrin', 'Neil Armstrong', 3);
INSERT INTO Intrebari VALUES (27, 'Caderea Zidului Berlinului?', '1989', '1991', '1980', 1);
INSERT INTO Intrebari VALUES (28, 'Anul Indepedentei SUA?', '1776', '1800', '1650', 1);
INSERT INTO Intrebari VALUES (29, 'Domnitorul Moldovei timp de 47 de ani?', 'Mircea cel Batran', 'Mihai Viteazul', 'Stefan cel Mare', 3);
INSERT INTO Intrebari VALUES (30, 'Batalia de la Waterloo a fost pierduta de?', 'Napoleon', 'Wellington', 'Cezar', 1);

INSERT INTO Intrebari VALUES (31, 'Anul Marii Uniri?', '1859', '1918', '1945', 2);
INSERT INTO Intrebari VALUES (32, 'Cel mai inalt varf din Romania?', 'Omu', 'Negoiu', 'Moldoveanu', 3);
INSERT INTO Intrebari VALUES (33, 'Fluviul care trece prin sudul tarii?', 'Olt', 'Mures', 'Dunarea', 3);
INSERT INTO Intrebari VALUES (34, 'In ce an a intrat Romania in UE?', '2004', '2007', '2010', 2);
INSERT INTO Intrebari VALUES (35, 'Orasul Capitala Culturala Europeana 2007?', 'Sibiu', 'Cluj', 'Brasov', 1);
INSERT INTO Intrebari VALUES (36, 'Castelul asociat cu Dracula?', 'Peles', 'Bran', 'Corvinilor', 2);
INSERT INTO Intrebari VALUES (37, 'Cine a scris versurile imnului?', 'Andrei Muresanu', 'Anton Pann', 'George Cosbuc', 1);
INSERT INTO Intrebari VALUES (38, 'Ce mare are iesire Romania?', 'Marea Rosie', 'Marea Neagra', 'Marea Mediterana', 2);
INSERT INTO Intrebari VALUES (39, 'Moneda nationala a Romaniei?', 'Euro', 'Leu', 'Dolar', 2);
INSERT INTO Intrebari VALUES (40, 'Judetul cu capitala la Iasi?', 'Vaslui', 'Iasi', 'Neamt', 2);

INSERT INTO Intrebari VALUES (41, 'Cine a scris "Luceafarul"?', 'Creanga', 'Caragiale', 'Eminescu', 3);
INSERT INTO Intrebari VALUES (42, 'Autorul romanului "Ion"?', 'Liviu Rebreanu', 'Marin Preda', 'Camil Petrescu', 1);
INSERT INTO Intrebari VALUES (43, 'Cine a scris "Romeo si Julieta"?', 'Shakespeare', 'Dante', 'Goethe', 1);
INSERT INTO Intrebari VALUES (44, 'Autorul seriei "Harry Potter"?', 'Tolkien', 'J.K. Rowling', 'George R.R. Martin', 2);
INSERT INTO Intrebari VALUES (45, 'Cine a scris "Amintiri din copilarie"?', 'Ion Creanga', 'Mihai Eminescu', 'Ioan Slavici', 1);
INSERT INTO Intrebari VALUES (46, 'Personajul principal din "Morometii"?', 'Vitoria Lipan', 'Ilie Moromete', 'Ion', 2);
INSERT INTO Intrebari VALUES (47, 'Cine a scris "Divina Comedie"?', 'Dante Alighieri', 'Petrarca', 'Machiavelli', 1);
INSERT INTO Intrebari VALUES (48, 'Romanul "Mandrie si Prejudecata"?', 'Emily Bronte', 'Jane Austen', 'Virginia Woolf', 2);
INSERT INTO Intrebari VALUES (49, 'Cine a scris "Baltagul"?', 'Mihail Sadoveanu', 'Liviu Rebreanu', 'George Calinescu', 1);
INSERT INTO Intrebari VALUES (50, 'Autorul "Stapanul Inelelor"?', 'J.R.R. Tolkien', 'C.S. Lewis', 'H.G. Wells', 1);

COMMIT;
