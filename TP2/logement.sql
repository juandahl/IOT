---------------------------------- Drop Tables ------------------------------------

DROP TABLE Logements;
DROP TABLE Pieces;
DROP TABLE TypeCapteurs;
DROP TABLE Capteurs;
DROP TABLE Mesure;
DROP TABLE Facture; 

---------------------------------- Create Table ----------------------------------

-- Create logements table
CREATE TABLE Logements (
    id_logement INTEGER PRIMARY KEY AUTOINCREMENT,
    address varchar(255),
    telephone varchar(255),
    ip varchar(255),
    date_insertion TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


-- Create Pieces table
CREATE TABLE Pieces (
    id_piece INTEGER PRIMARY KEY AUTOINCREMENT,
    x INTEGER,
    y INTEGER,
    z INTEGER,
    id_logement INTEGER,

	-- Add foreign key to Pieces table. Reference from Pieces to Logements
    FOREIGN KEY (id_logement) REFERENCES Logements(id_logement)
);



-- Create TypeCapteurs table
CREATE TABLE TypeCapteurs (
    id_type INTEGER PRIMARY KEY AUTOINCREMENT,
    unite varchar(255),
    precision INTEGER
);


-- Create capteurs table
CREATE TABLE Capteurs (
    id_capteur INTEGER PRIMARY KEY AUTOINCREMENT,
    port varchar(255),
    date_insertion TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    id_piece INTEGER,
    id_type INTEGER,

	-- Add foreign key to Capteurs table. Reference from Capteurs to Pieces
    FOREIGN KEY (id_piece) REFERENCES Pieces(id_piece),

	-- Add foreign key to Capteurs table. Reference from Capteur to typeCapteurs
	FOREIGN KEY (id_type) REFERENCES TypeCapteurs(id_type)

);

-- Create Mesure table
CREATE TABLE Mesure (
    id_mesure INTEGER PRIMARY KEY AUTOINCREMENT,
    valeur INTEGER,
    date_insertion TIMESTAMP,
    id_capteur INTEGER,

    -- Add foreign key to Mesure table. Reference from Mesure to Capteurs
	FOREIGN KEY (id_capteur) REFERENCES Capteurs(id_capteur)
);



-- Create Facture table
CREATE TABLE Facture (
    id_facture INTEGER PRIMARY KEY AUTOINCREMENT,
    date_insertion TIMESTAMP,
    montant INTEGER,
    valeur_consumee INTEGER,
    type_facture varchar(255),
    id_logement INTEGER,

	-- Add foreign key to Mesure table. Reference from Mesure to Capteurs
	FOREIGN KEY (id_logement) REFERENCES Logements(id_logement)

);


---------------------------------- Insert tuples ----------------------------------

-- Insert 1 tuple for Logements table
INSERT INTO Logements(address, telephone, ip) VALUES('5 Boulevard Jourdan', '0766536491', '192.168.0.1');

-- Insert 4 tuples for pieces table
INSERT INTO Pieces(x, y, z, id_logement) VALUES(45, 90, 76, 1);
INSERT INTO Pieces(x, y, z, id_logement) VALUES(56, 14, 82, 1);
INSERT INTO Pieces(x, y, z, id_logement) VALUES(23, 1, 100, 1);
INSERT INTO Pieces(x, y, z, id_logement) VALUES(142, 54, 36, 1);

-- Insert 2 tuples for TypeCapteurs table
INSERT INTO TypeCapteurs(unite, precision) VALUES('degree', 2);
INSERT INTO TypeCapteurs(unite, precision) VALUES('degree', 4);
INSERT INTO TypeCapteurs(unite, precision) VALUES('degree', 6);
INSERT INTO TypeCapteurs(unite, precision) VALUES('degree', 8);

-- Insert 2 tuples for Capteurs table
INSERT INTO Capteurs(port, id_piece, id_type) VALUES('D080', 1, 2);
INSERT INTO Capteurs(port, id_piece, id_type) VALUES('D080', 3, 4);

-- Insert 2 tuples for Mesure table
INSERT INTO Mesure(valeur, id_capteur) VALUES(20, 1);
INSERT INTO Mesure(valeur, id_capteur) VALUES(15, 2);

-- Insert 4 tuples for Facture table
INSERT INTO Facture(montant, valeur_consumee, type_facture, id_logement) VALUES(150, 45000, 'electricite', 1);
INSERT INTO Facture(montant, valeur_consumee, type_facture, id_logement) VALUES(25, 1000, 'eau', 1);
INSERT INTO Facture(montant, valeur_consumee, type_facture, id_logement) VALUES(2000, 700, 'dechets', 1);
INSERT INTO Facture(montant, valeur_consumee, type_facture, id_logement) VALUES(75, 4500, 'eau', 1);
