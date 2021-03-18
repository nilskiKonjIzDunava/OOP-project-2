#include "Circuit.h"



Circuit::Circuit()
{
	duration_ = 0;
}

Circuit::~Circuit()
{
	for (auto elem : digital_circuit_)
	{
		delete elem;
		elem = nullptr;
	}

	digital_circuit_.clear();
}

Circuit::Circuit(const Circuit & other)
{
	Element::elem_type type;
	Element* new_elem = nullptr;
	Element* ell = nullptr;
	int i = 0, m = 0, n = 0;

	critical_moments_ = other.critical_moments_;

	// prvo pravimo novi niz, pa cemo uvezati elemente
	for (auto el : other.digital_circuit_)
	{
		type = el->getType();
		switch (type)
		{
		case Element::SCOPE:
		{
			new_elem = new Scope(el->getId(), Element::SCOPE, 1);
			digital_circuit_.push_back(new_elem);
			break;
		}

		case Element::PERIODIC:
		{
			Periodic* temp_periodic = dynamic_cast<Periodic*> (other.digital_circuit_[i]);
			new_elem = new Periodic(temp_periodic->getId(), Element::PERIODIC, temp_periodic->getFrequency());
			digital_circuit_.push_back(new_elem);
			break;
		}
		case Element::APERIODIC:
		{
			Aperiodic* temp_periodic = dynamic_cast<Aperiodic*> (other.digital_circuit_[i]);
			new_elem = new Aperiodic(temp_periodic->getId(), Element::APERIODIC, temp_periodic->getNumOfToggles(), temp_periodic->getMomentsOfToggles());
			digital_circuit_.push_back(new_elem);
			break;
		}
		case Element::NOT:
		{
			new_elem = new NotGate(el->getId(), Element::NOT, 1);
			digital_circuit_.push_back(new_elem);
			break;
		}
		case Element::AND:
		{
			new_elem = new AndGate(el->getId(), Element::AND, el->getNumOfInputs());
			digital_circuit_.push_back(new_elem);
			break;
		}
		case Element::OR:
		{
			new_elem = new OrGate(el->getId(), Element::OR, el->getNumOfInputs());
			digital_circuit_.push_back(new_elem);
			break;
		}

		}

	}
	// sada povezujemo elemente
	for (auto el : other.digital_circuit_)
	{
		if ((el->getNumOfInputs()) != 0)
		{
			for (auto inp : (el->getInputs()))
			{
				// zapamtimo id elementa ciji izlaz povezujemo sa trenutnim inputom na poziciji n
				int idd = el->getId();
				// moramo proci kroz kopirani niz i pronaci trazeni id 
				for (auto ell = digital_circuit_.begin(); (((*ell)->getId()) == idd) && (ell != digital_circuit_.end()); ell++)     // ISUSE HRISTE
				{
					// kada ga najemo povezujemo pronadjeni element sa odgovarajucim id-om za n-ti ulaz m-tog elementa 
					(digital_circuit_[m]->getInputs())[n] = *ell;
				}
				n++;

			}
		}
		else
			continue;
		// u svakom slucaju inkrementiramo m koji ce nam pokazati kojoj poziciji novog Circuit-a treba da pristupimo (isto su elementi rasporedjeni)
		m++;
	}
}

Circuit::Circuit(Circuit && other)
{
	for (auto it = other.digital_circuit_.begin(); it != other.digital_circuit_.end(); it++) {
		digital_circuit_.push_back(*it);
		*it = nullptr;
	}
	for (auto cm : other.critical_moments_)
	{
		critical_moments_.push_back(cm);
	}
	other.critical_moments_.clear();
}

int countWords(string str)
{
	stringstream s(str);
	string word;

	int count = 0;
	while (s >> word)
		count++;
	return count;
}
bool IsNewCriticalMoment(double moment, vector <double> & critical_moments)
{
	int N = critical_moments.size();
	// ako smo tek poceli sa ucitavanjem kriticnih trenutaka onda prvo stavljamo 0 u kriticne, pa zato vratimo true
	if (N == 0) return true;
	bool test = true;
	for (auto t : critical_moments)
	{
		if (t == moment)
		{
			test = false;
			break;
		}
	}
	if (test)
		return true;
	else return false;
}

bool AreAllInputsConnected(vector<Element*>& digital_circuit)
{
	for (auto dc : digital_circuit)
	{
		for (auto inp : dc->getInputs())
		{
			if (!inp)
				return false;
		}
	}
	return true;
}


void Circuit::loadOneCircuit(const string & filepath)
{
	fstream input_file(filepath, ios::in);
	string line;
	// sluzi za pamcenje prve dve linije, da izbegnemo dodeljivanje vrednosti vise puta,
	// kao i da odvojimo od citanja ostatka fajla koji ima drugaciji karakteristican zapis
	queue<double> fst_sec_line;
	//u ovaj vekror smestamo id-ove elemenata, pa kod unosa svakog narednog proveravamo da li je taj id vec zauzet
	vector<int>ids;
	// sluzi za brojanje linija nakon sto smo procitali prve dve
	unsigned int line_cnt = 0;
	bool num_el_time_read = false;
	try
	{
		while (getline(input_file, line) && (line.size() != 0))
		{
			//pomocni stringstream za cuvanje linije
			stringstream ss;
			//ucitavanje linije
			ss << line;
			if ((fst_sec_line.size() == 2) && (num_el_time_read == false))
			{
				try
				{
					if (fst_sec_line.front() <= 0)
						throw new NegativeTimeException("Greska! Vreme simulacije mora biti pozitivan broj! Bice uneto podrazumevano vreme od 10 us.");
					duration_ = fst_sec_line.front();
				}
				catch (NegativeTimeException* e)
				{
					cout << e->what() << endl;
					duration_ = 10;
					delete e;
				}

				if (fst_sec_line.back() < 0)
					throw new NegativeNumberOfElementsException("Greska! Broj elemenata u kolu ne moze biti negativan! Kolo ce biti izbrisano.");
				num_of_elem_ = fst_sec_line.back();
				num_el_time_read = true;
			}
			if (countWords(ss.str()) == 1)
				fst_sec_line.push(stod(ss.str()));

			else
			{
				line_cnt++;
				if (line_cnt > fst_sec_line.back())
				{
					// ovde ce uci kad dodje do drugog dela fajla, kada krece opisivanje veza izmedju elemenata
					int temp_inp, inp_of, ordinal;
					ss >> temp_inp >> inp_of >> ordinal;

					bool flag_from = false, flag_to = false;
					Element* ptr_from = nullptr;
					Element* ptr_to = nullptr;
					// prolaskom kroz sve elemente kola cuvamo pokazivace na dva elementa u vezi
					for (auto e : digital_circuit_)
					{
						if (e->getId() == inp_of)
						{
							flag_from = true;
							ptr_from = e;
						}
						if (e->getId() == temp_inp)
						{
							flag_to = true;
							ptr_to = e;
						}
						if (flag_from && flag_to)
						{
							if (!((ptr_from->getInputs())[ordinal]))
							{
								(ptr_from->getInputs())[ordinal] = ptr_to;
								break;
							}
							else
								throw new AlreadyConnectedPortException("Greska! Ne moze se dodati vise elemenata na isti ulaz. Kolo ce biti obrisano.");
							
						}

					}
					ptr_from = ptr_to = nullptr;

				}
				else
				{

					int temp_id, temp_type;
					ss >> temp_id >> temp_type;


					if (ids.size())
					{
						for (int j = 0; j < ids.size(); j++)
						{
							if (ids[j] == temp_id)
								throw new NotUniqueIdException("Greska! Unesen je id koji vec postoji u bazi, kolo ce biti izbrisano.");
						}
						ids.push_back(temp_id);
					}
					else
						ids.push_back(temp_id);

					switch (temp_type)
					{
					case 0:
					{
						Element* ptr0 = new Scope(temp_id, Element::SCOPE, 1);
						digital_circuit_.push_back(ptr0);
						break;
					}
					case 1:
					{
						double temp_freq;
						ss >> temp_freq;
						if (temp_freq < 0)
						{
							throw new NegativeFrequencyException("Greska! Frekvencija ne moze biti negativna. Kolo ce biti obrisano.");
						}



						try
						{
							if (temp_freq == 0)
								throw new ZeroFrequencyException("Greska! Unesena je 0 za frekvenciju. Podrazumevana vrednost frekvencije ce biti 1 MHz.");
						}
						catch (ZeroFrequencyException* e)
						{
							cout << e->what() << endl;
							temp_freq = 1;
							delete e;
						}
						Element* ptr1 = new Periodic(temp_id, Element::PERIODIC, temp_freq);
						digital_circuit_.push_back(ptr1);
						double time = 0;
						while (time < duration_)
						{

							if ((IsNewCriticalMoment(time, critical_moments_)) == true)
								critical_moments_.push_back(time);
							time = time + 1 / (2 * temp_freq);
						}

						break;
					}

					case 2:
					{
						double temp_toggles;

						vector<double>temp_togg_vector;

						while (ss >> temp_toggles)
						{
							temp_togg_vector.push_back(temp_toggles);
						}
						int temp_num_of_toggles = 0;
						temp_num_of_toggles = temp_togg_vector.size();
						Element* ptr2 = new Aperiodic(temp_id, Element::APERIODIC, temp_num_of_toggles, temp_togg_vector);
						digital_circuit_.push_back(ptr2);
						double time = 0;
						//critical_moments_.push_back(time);
						if (((IsNewCriticalMoment(time, critical_moments_)) == true) && (time <= duration_))
							critical_moments_.push_back(time);
						////////////////
						vector<double> cm;
						cm = this->getCriticalMoments();
						//////////////////
						if (temp_num_of_toggles != 0)
						{
							for (int i = 0; i < temp_num_of_toggles; i++)
							{
								time = time + temp_togg_vector[i];
								if (((IsNewCriticalMoment(time, critical_moments_)) == true) && (time <= duration_))		
									critical_moments_.push_back(time);
								else 
									continue;

							}
						}
						break;
					}
					case 3:
					{
						Element* ptr3 = new NotGate(temp_id, Element::NOT, 1);
						digital_circuit_.push_back(ptr3);
						break;
					}
					case 4:
					{
						int temp_num_inp;
						Element*ptr4;
						if (ss >> temp_num_inp)
							ptr4 = new AndGate(temp_id, Element::AND, temp_num_inp);
						else
							ptr4 = new AndGate(temp_id, Element::AND, 2);
						digital_circuit_.push_back(ptr4);
						break;

					}
					case 5:
					{
						int temp_num_inp;
						Element* ptr5;
						if (ss >> temp_num_inp)
							ptr5 = new OrGate(temp_id, Element::OR, temp_num_inp);
						else
							ptr5 = new OrGate(temp_id, Element::OR, 2);
						digital_circuit_.push_back(ptr5);
						break;
					}
					}
				}



			}

		}
		sort(critical_moments_.begin(), critical_moments_.end());
		
		// pre izlaska treba proveriti da li postoji ulaz nekog elementa (koji nije generator) koji nije povezan - "viseci"
		// ako postoji bacamo izuzetak i brisemo kolo
		if (AreAllInputsConnected(digital_circuit_) == false)
			throw NotConnectedPorts("Greska! Svi ulazi elemeneta moraju biti povezani. Uneseno kolo ce se unistiti.");
	}
	catch (NotUniqueIdException* e)
	{
		cout << e->what() << endl;
		input_file.close();
		//delete e;
		throw;

	}
	catch (NegativeNumberOfElementsException* e)
	{
		cout << e->what() << endl;
		input_file.close();
		throw;
	}
	catch (NegativeFrequencyException* e)
	{
		cout << e->what() << endl;
		input_file.close();
		throw;
	}
	catch (NotConnectedPorts* e)
	{
		cout << e->what() << endl;
		input_file.close();
		throw;
	}
	catch (AlreadyConnectedPortException* e)
	{
		cout << e->what() << endl;
		input_file.close();
		throw;
	}

	
	input_file.close();
}

void Circuit::simulateCircuit(const string & filepath)
{
	// post-order obilaskom pravimo izlazni niz koji ce kao prvi element imati sondu:
	// kako se krecemo nizom od poslednjeg elementa (unazad), oni su rasporedjeni tako da:
	// prvo ide levo podstablo, pocevsi od generatora,
	// pa onda element za ciji je ulaz nakacen taj generator
	// desno podstablo i negovi ulazi
	// pa sve tako do sonde koja ce biti prvi element niza
	// za sledecu sondu se isto tako obilazi podstablo od interesa
	// ispisuju se vrednosti za svaki kriticni trenutak

	stack<Element*> temp_stack;
	vector<Element*> postorder_elem;
	vector<Element::state>scope_output;
	int i = 0;
	for (auto curr_elem : digital_circuit_)
	{
		// prvo pronalazimo sondu u nizu elemenata, nju bacamo na pocetak post-oreder niza
		if (curr_elem->getType() == (Element::SCOPE))
		{

			int cnt_elem = 0;
			temp_stack.push(curr_elem);
			while (!(temp_stack.empty()))
			{
				i = 0;
				Element* elem = temp_stack.top();
				temp_stack.pop();
				postorder_elem.push_back(elem);
				i = 0;
				cnt_elem++;
				for (auto el : (elem->getInputs()))
					temp_stack.push(el);
			}

			// OVO IZBRISI, SAMO DA VIDIM DA LI SU DOBRO RASPOREDJENI KRITICNI, ZAKLJUCANO JE TO POLJE U DEBAGERU
			vector<double> sorted_cm;
			sorted_cm = this->getCriticalMoments();
			// kako su svi elementi rasporedjeni u post-order redosledu, za svaki kritican prenutak prolazimo nizom elemenata i sonda dobija vrednost za taj trenutak
			for (auto cm : sorted_cm)
			{
				for (i = cnt_elem - 1; i >= 0; i--)
					postorder_elem[i]->processLogic(cm);
				scope_output.push_back(postorder_elem[0]->getCurrState());
			}
			// ispis u izlazni
			writeToFile(filepath, postorder_elem[0]->getId(), scope_output);
			scope_output.clear();
			postorder_elem.clear();
		}
		else
			continue;
	}

}

void Circuit::writeToFile(const string & filepath, int scope_id, vector<Element::state>& output)
{
	// prolazi se paralelno nizovima kriticnih trenutaka i vrednosti sonde
	// imace isti broj elemenata, pa cemo samo gledati promenu vrednosti,
	// a na istoj poziciji ce se nalaziti i odg kriticni trenutak
	string new_name = filepath;
	unsigned int len_without_txt = filepath.length() - 4;
	string idd = to_string(scope_id);
	new_name.insert(len_without_txt, "_" + idd);
	fstream output_file;
	output_file.open(new_name, ios::out);

	if (output[0] == Element::ACTIVE)
		output_file << "0 -> 1: " << critical_moments_[0] << "us" << endl;

	for (int i = 1; i < output.size(); i++)
	{
		if (critical_moments_[i] == duration_) break;    // ovo ako ne treba da se ispisuje promena ako se desila bas u poslednjoj sekundi simulacije

		if ((output[i - 1] == Element::INACTIVE) && (output[i] == Element::ACTIVE))
		{
			output_file << "0 -> 1: " << critical_moments_[i] << "us" << endl;
		}
		else if ((output[i - 1] == Element::ACTIVE) && (output[i] == Element::INACTIVE))
		{
			output_file << "1 -> 0: " << critical_moments_[i] << "us" << endl;
		}
		else
			continue;
	}
	output_file.close();

}

vector<double>& Circuit::getCriticalMoments()
{
	return critical_moments_;
}


