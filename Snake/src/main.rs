use eframe::egui;
use rand::Rng;
use std::fs;

#[derive(serde::Serialize, serde::Deserialize)]
struct Scoruri {
    valori: Vec<i32>,
}

#[derive(Clone, Copy, PartialEq)]
struct Coord {
    x: i32,
    y: i32,
}

struct Joc {
    sarpe: Vec<Coord>,
    mancare: Coord,
    scor: i32,
    directie: i32,
    gata: bool,
    timp: f32,
    pauza: bool,
    viteza: f32,
    obstacole: Vec<Coord>,
    top: Vec<i32>,
    arata_top: bool,
    scor_salvat: bool,
    directie_schimbata: bool,
}

impl Joc {
    fn genereaza_mancare(&self, latime: i32, inaltime: i32) -> Coord {
        let mut rng = rand::rng();
        loop {
            let c = Coord {
                x: rng.random_range(0..latime),
                y: rng.random_range(0..inaltime),
            };

            if !self.obstacole.contains(&c) && !self.sarpe.contains(&c) {
                return c;
            }
        }
    }

    fn new(_cc: &eframe::CreationContext<'_>) -> Self {
        let top = match fs::read_to_string("scoruri.json") {
            Ok(continut) => {
                if let Ok(date) = serde_json::from_str::<Scoruri>(&continut) {
                    date.valori
                } else {
                    vec![]
                }
            }
            Err(_) => vec![],
        };

        let sarpe_start = vec![
            Coord { x: 2, y: 3 },
            Coord { x: 2, y: 4 },
            Coord { x: 2, y: 5 },
        ];

        let mut rng = rand::rng();
        let mut obs = Vec::new();
        for _ in 0..50 {
            let start_x = rng.random_range(3..60);
            let start_y = rng.random_range(3..35);
            let orizontal = rng.random_range(0..2) == 1;

            for k in 0..5 {
                let cx;
                let cy;
                if orizontal {
                    cx = start_x + k;
                    cy = start_y;
                } else {
                    cx = start_x;
                    cy = start_y + k;
                }
                obs.push(Coord { x: cx, y: cy });
            }
        }

        let mancare_start = loop {
            let c = Coord {
                x: rng.random_range(1..60),
                y: rng.random_range(1..35),
            };

            if !obs.contains(&c) && !sarpe_start.contains(&c) {
                break c;
            }
        };

        Self {
            sarpe: sarpe_start,
            mancare: mancare_start,
            scor: 0,
            directie: 0,
            gata: false,
            timp: 0.0,
            pauza: false,
            viteza: 0.2,
            obstacole: obs,
            top,
            arata_top: false,
            scor_salvat: false,
            directie_schimbata: false,
        }
    }

    fn salveaza_scoruri(&self) {
        let date = Scoruri {
            valori: self.top.clone(),
        };

        if let Ok(json) = serde_json::to_string(&date) {
            let _ = fs::write("scoruri.json", json);
        }
    }
}

impl eframe::App for Joc {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        if !self.gata && ctx.input(|i| i.key_pressed(egui::Key::Space)) {
            self.pauza = !self.pauza;
        }

        egui::TopBottomPanel::top("header").show(ctx, |ui| {
            ui.vertical_centered(|ui| {
                ui.heading("SNAKE");
                ui.label(
                    egui::RichText::new(format!("Scor: {}", self.scor))
                        .size(18.0)
                        .strong(),
                );
            });
        });

        egui::CentralPanel::default()
            .frame(egui::Frame::new().fill(egui::Color32::from_rgb(136, 231, 136)))
            .show(ctx, |ui| {
                let zona_joc = ui.available_rect_before_wrap();
                let marime = 20.0;

                let latime = (zona_joc.width() / marime) as i32;
                let inaltime = (zona_joc.height() / marime) as i32;

                let inaltime_reala_joc = inaltime as f32 * marime;
                let offset_y = (zona_joc.height() - inaltime_reala_joc) / 2.0;

                let start_x = zona_joc.min.x;
                let start_y = zona_joc.min.y + offset_y;

                if !self.pauza && !self.directie_schimbata {
                    let mut noua_directie = self.directie;

                    if ctx
                        .input(|i| i.key_pressed(egui::Key::ArrowUp) || i.key_pressed(egui::Key::W))
                    {
                        if self.directie != 1 {
                            noua_directie = 0;
                        }
                    } else if ctx.input(|i| {
                        i.key_pressed(egui::Key::ArrowDown) || i.key_pressed(egui::Key::S)
                    }) {
                        if self.directie != 0 {
                            noua_directie = 1;
                        }
                    } else if ctx.input(|i| {
                        i.key_pressed(egui::Key::ArrowLeft) || i.key_pressed(egui::Key::A)
                    }) {
                        if self.directie != 3 {
                            noua_directie = 2;
                        }
                    } else if ctx.input(|i| {
                        i.key_pressed(egui::Key::ArrowRight) || i.key_pressed(egui::Key::D)
                    }) && self.directie != 2
                    {
                        noua_directie = 3;
                    }

                    if noua_directie != self.directie {
                        self.directie = noua_directie;
                        self.directie_schimbata = true;
                    }
                }

                if !self.gata && !self.pauza {
                    self.timp += ctx.input(|i| i.stable_dt);
                    if self.timp > self.viteza {
                        self.timp = 0.0;
                        self.directie_schimbata = false;
                        let mut cap_nou = self.sarpe[0];
                        match self.directie {
                            0 => cap_nou.y -= 1,
                            1 => cap_nou.y += 1,
                            2 => cap_nou.x -= 1,
                            3 => cap_nou.x += 1,
                            _ => {}
                        }

                        if cap_nou.x < 0
                            || cap_nou.x >= latime
                            || cap_nou.y < 0
                            || cap_nou.y >= inaltime
                            || self.sarpe.contains(&cap_nou)
                            || self.obstacole.contains(&cap_nou)
                        {
                            self.gata = true;
                        } else {
                            self.sarpe.insert(0, cap_nou);
                            if cap_nou == self.mancare {
                                self.scor += 1;

                                self.viteza = (self.viteza - 0.02).max(0.07);
                                self.mancare = self.genereaza_mancare(latime, inaltime);
                            } else {
                                self.sarpe.pop();
                            }
                        }
                    }
                }

                if self.gata {
                    if !self.scor_salvat {
                        self.top.push(self.scor);
                        self.top.sort_by(|a, b| b.cmp(a));
                        self.top.truncate(5);
                        self.salveaza_scoruri();
                        self.scor_salvat = true;
                    }

                    egui::Window::new("Game Over")
                        .collapsible(false)
                        .resizable(false)
                        .anchor(egui::Align2::CENTER_CENTER, egui::vec2(0.0, 0.0))
                        .show(ctx, |ui| {
                            ui.heading("Ai Pierdut!");
                            ui.label(format!("Scor final: {}", self.scor));
                            ui.label("Apasa SPACE pentru Restart");
                            ui.add_space(10.0);

                            if ui.button("Top Scoruri").clicked() {
                                self.arata_top = !self.arata_top;
                            }

                            ui.add_space(10.0);
                            ui.label("Apasa SPACE pentru Restart");
                        });

                    if ctx.input(|i| i.key_pressed(egui::Key::Space)) {
                        self.gata = false;
                        self.scor = 0;
                        self.directie = 0;
                        self.viteza = 0.2;
                        self.arata_top = false;
                        self.scor_salvat = false;
                        self.sarpe = vec![
                            Coord { x: 2, y: 3 },
                            Coord { x: 2, y: 4 },
                            Coord { x: 2, y: 5 },
                        ];
                        self.mancare = self.genereaza_mancare(latime, inaltime);
                    }
                }

                let painter = ui.painter();
                let to_screen = |x: i32, y: i32| {
                    egui::Rect::from_min_size(
                        egui::pos2(start_x + x as f32 * marime, start_y + y as f32 * marime),
                        egui::vec2(marime, marime),
                    )
                };

                for obs in &self.obstacole {
                    painter.rect_filled(
                        to_screen(obs.x, obs.y),
                        4.0,
                        egui::Color32::from_rgb(40, 50, 40),
                    );
                }

                painter.rect_filled(
                    to_screen(self.mancare.x, self.mancare.y),
                    5.0,
                    egui::Color32::RED,
                );

                for punct in &self.sarpe {
                    painter.rect_filled(to_screen(punct.x, punct.y), 2.0, egui::Color32::BLUE);
                }
            });

        if self.pauza && !self.gata {
            egui::Window::new("Pauza")
                .collapsible(false)
                .resizable(false)
                .anchor(egui::Align2::CENTER_CENTER, egui::vec2(0.0, 0.0))
                .show(ctx, |ui| {
                    ui.vertical_centered(|ui| {
                        ui.add_space(10.0);
                        ui.label("Apasa SPACE pentru a continua");

                        ui.add_space(10.0);
                        if ui.button("Continua").clicked() {
                            self.pauza = false;
                        }
                    });
                });
        }

        if self.arata_top {
            egui::Window::new("Top 5 scoruri")
                .collapsible(false)
                .resizable(false)
                .anchor(egui::Align2::CENTER_TOP, egui::vec2(0.0, 150.0)) // O punem putin mai jos
                .show(ctx, |ui| {
                    ui.vertical_centered(|ui| {
                        for (i, s) in self.top.iter().enumerate() {
                            ui.label(format!("{}. {} puncte", i + 1, s));
                        }

                        if self.top.is_empty() {
                            ui.label("Niciun scor inregistrat inca.");
                        }
                    });
                });
        }

        ctx.request_repaint();
    }
}

fn main() -> eframe::Result {
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_fullscreen(true),
        ..Default::default()
    };
    eframe::run_native("Snake", options, Box::new(|cc| Ok(Box::new(Joc::new(cc)))))
}
