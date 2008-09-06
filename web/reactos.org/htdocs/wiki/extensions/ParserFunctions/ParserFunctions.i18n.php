<?php
/**
 * Internationalisation file for extension ParserFunctions.
 *
 * @addtogroup Extensions
*/

$messages = array();

$messages['en'] = array(
	'pfunc_desc'                            => 'Enhance parser with logical functions',
	'pfunc_time_error'                      => 'Error: invalid time',
	'pfunc_time_too_long'                   => 'Error: too many #time calls',
	'pfunc_rel2abs_invalid_depth'           => 'Error: Invalid depth in path: "$1" (tried to access a node above the root node)',
	'pfunc_expr_stack_exhausted'            => 'Expression error: Stack exhausted',
	'pfunc_expr_unexpected_number'          => 'Expression error: Unexpected number',
	'pfunc_expr_preg_match_failure'         => 'Expression error: Unexpected preg_match failure',
	'pfunc_expr_unrecognised_word'          => 'Expression error: Unrecognised word "$1"',
	'pfunc_expr_unexpected_operator'        => 'Expression error: Unexpected $1 operator',
	'pfunc_expr_missing_operand'            => 'Expression error: Missing operand for $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Expression error: Unexpected closing bracket',
	'pfunc_expr_unrecognised_punctuation'   => 'Expression error: Unrecognised punctuation character "$1"',
	'pfunc_expr_unclosed_bracket'           => 'Expression error: Unclosed bracket',
	'pfunc_expr_division_by_zero'           => 'Division by zero',
	'pfunc_expr_unknown_error'              => 'Expression error: Unknown error ($1)',
	'pfunc_expr_not_a_number'               => 'In $1: result is not a number',
	'pfunc_ifexist_warning'                 => 'Warning: This page contains too many #ifexist calls. It should have less than $2, there are now $1.',
	'pfunc_max_ifexist_category'            => 'Pages with too many ifexist calls',
);

/** Aragonese (Aragonés)
 * @author Juanpabl
 */
$messages['an'] = array(
	'pfunc_time_error'                      => 'Error: tiempo incorreuto',
	'pfunc_time_too_long'                   => 'Error: masiadas cridas #time',
	'pfunc_rel2abs_invalid_depth'           => 'Error: Fondura incorreuta en o path: "$1" (prebó d\'azeder ta un nodo por denzima d\'o nodo radiz)',
	'pfunc_expr_stack_exhausted'            => "Error d'espresión: Pila acotolada",
	'pfunc_expr_unexpected_number'          => "Error d'espresión: numbero no asperato",
	'pfunc_expr_preg_match_failure'         => "Error d'espresión: fallo de preg_match no asperato",
	'pfunc_expr_unrecognised_word'          => 'Error d\'espresión: palabra "$1" no reconoixita',
	'pfunc_expr_unexpected_operator'        => "Error d'espresión: operador $1 no asperato",
	'pfunc_expr_missing_operand'            => "Error d'espresión: á $1 li falta un operando",
	'pfunc_expr_unexpected_closing_bracket' => "Error d'espresión: zarradura d'o gafet no asperata",
	'pfunc_expr_unrecognised_punctuation'   => 'Error d\'espresión: caráuter de puntuazión "$1" no reconoixito',
	'pfunc_expr_unclosed_bracket'           => "Error d'espresión: gafet sin zarrar",
	'pfunc_expr_division_by_zero'           => 'Dibisión por zero',
	'pfunc_expr_unknown_error'              => "Error d'espresión: error esconoixito ($1)",
	'pfunc_expr_not_a_number'               => 'En $1: o resultau no ye un numero',
	'pfunc_ifexist_warning'                 => 'Pare cuenta: ista pachina contiene masiadas cridas #ifexist. Bi ha $1, y caldría que tenese menos de $2',
	'pfunc_max_ifexist_category'            => 'Pachinas con masiadas cridas ifexist',
);

/** Arabic (العربية)
 * @author Meno25
 * @author Siebrand
 */
$messages['ar'] = array(
	'pfunc_desc'                            => 'بارسر ممدد بدوال منطقية',
	'pfunc_time_error'                      => 'خطأ: زمن غير صحيح',
	'pfunc_time_too_long'                   => 'خطأ: too many #time calls',
	'pfunc_rel2abs_invalid_depth'           => 'خطأ: عمق غير صحيح في المسار: "$1" (حاول دخول عقدة فوق العقدة الجذرية)',
	'pfunc_expr_stack_exhausted'            => 'خطأ في التعبير: ستاك مجهد',
	'pfunc_expr_unexpected_number'          => 'خطأ في التعبير: رقم غير متوقع',
	'pfunc_expr_preg_match_failure'         => 'خطأ في التعبير: فشل preg_match غير متوقع',
	'pfunc_expr_unrecognised_word'          => 'خطأ في التعبير: كلمة غير متعرف عليها "$1"',
	'pfunc_expr_unexpected_operator'        => 'خطأ في التعبير: عامل $1 غير متوقع',
	'pfunc_expr_missing_operand'            => 'خطأ في التعبير: operand مفقود ل$1',
	'pfunc_expr_unexpected_closing_bracket' => 'خطأ في التعبير: قوس إغلاق غير متوقع',
	'pfunc_expr_unrecognised_punctuation'   => 'خطأ في التعبير: علامة ترقيم غير متعرف عليها "$1"',
	'pfunc_expr_unclosed_bracket'           => 'خطأ في التعبير: قوس غير مغلق',
	'pfunc_expr_division_by_zero'           => 'القسمة على صفر',
	'pfunc_expr_unknown_error'              => 'خطأ في التعبير: خطأ غير معروف ($1)',
	'pfunc_expr_not_a_number'               => 'في $1: النتيجة ليست رقما',
	'pfunc_ifexist_warning'                 => 'تحذير: هذه المقالة تحتوي على #ifexist calls كثيرة جدا. ينبغي أن تحتوي على أقل من $2، يوجد الآن $1.',
	'pfunc_max_ifexist_category'            => 'الصفحات التي تحتوي على ifexist calls كثيرة جدا',
);

/** Asturian (Asturianu)
 * @author Esbardu
 */
$messages['ast'] = array(
	'pfunc_time_error'                      => 'Error: tiempu non válidu',
	'pfunc_time_too_long'                   => 'Error: demasiaes llamaes #time',
	'pfunc_rel2abs_invalid_depth'           => 'Error: Nivel de subdireutoriu non válidu: "$1" (intentu d\'accesu penriba del direutoriu raíz)',
	'pfunc_expr_stack_exhausted'            => "Error d'espresión: Pila escosada",
	'pfunc_expr_unexpected_number'          => "Error d'espresión: Númberu inesperáu",
	'pfunc_expr_preg_match_failure'         => "Error d'espresión: Fallu inesperáu de preg_match",
	'pfunc_expr_unrecognised_word'          => 'Error d\'espresión: Pallabra "$1" non reconocida',
	'pfunc_expr_unexpected_operator'        => "Error d'espresión: Operador $1 inesperáu",
	'pfunc_expr_missing_operand'            => "Error d'espresión: Falta operador en $1",
	'pfunc_expr_unexpected_closing_bracket' => "Error d'espresión: Paréntesis final inesperáu",
	'pfunc_expr_unrecognised_punctuation'   => 'Error d\'espresión: Caráuter de puntuación "$1" non reconocíu',
	'pfunc_expr_unclosed_bracket'           => "Error d'espresión: Paréntesis non zarráu",
	'pfunc_expr_division_by_zero'           => 'División por cero',
	'pfunc_expr_unknown_error'              => "Error d'espresión: Error desconocíu ($1)",
	'pfunc_expr_not_a_number'               => 'En $1: el resultáu nun ye un númberu',
	'pfunc_ifexist_warning'                 => 'Avisu: Esta páxina contién demasiaes llamaes #ifexist. Habría tener menos de $2, y tien anguaño $1.',
	'pfunc_max_ifexist_category'            => 'Páxines con demasiaes llamaes ifexist',
);

/** Bulgarian (Български)
 * @author Spiritia
 * @author DCLXVI
 */
$messages['bg'] = array(
	'pfunc_time_error'                      => 'Грешка: невалидно време',
	'pfunc_time_too_long'                   => 'Грешка: Твърде много извиквания на #time',
	'pfunc_expr_stack_exhausted'            => 'Грешка в записа: Стекът е изчерпан',
	'pfunc_expr_unexpected_number'          => 'Грешка в записа: Неочаквано число',
	'pfunc_expr_unrecognised_word'          => 'Грешка в записа: Неразпозната дума "$1"',
	'pfunc_expr_unexpected_operator'        => 'Грешка в записа: Неочакван оператор $1',
	'pfunc_expr_missing_operand'            => 'Грешка в записа: Липсващ операнд в $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Грешка в записа: Една затваряща скоба в повече',
	'pfunc_expr_unrecognised_punctuation'   => 'Грешка в записа: Неразпознат пунктуационен знак "$1"',
	'pfunc_expr_unclosed_bracket'           => 'Грешка в записа: Незатворена скоба',
	'pfunc_expr_division_by_zero'           => 'Деление на нула',
	'pfunc_expr_unknown_error'              => 'Грешка в записа: Неразпозната грешка ($1)',
	'pfunc_expr_not_a_number'               => 'В $1: резултатът не е число',
	'pfunc_ifexist_warning'                 => 'Внимание: Тази страница съдържа твърде много извиквания на #ifexist. Броят им сега е $1, а трябва да бъдат не повече от $2.',
	'pfunc_max_ifexist_category'            => 'Страници с твърде много извиквания на #ifexist',
);

/** Bengali (বাংলা)
 * @author Zaheen
 * @author Bellayet
 */
$messages['bn'] = array(
	'pfunc_desc'                            => 'লজিকাল ফাংশন দিয়ে পার্সারকে উন্নত করুন',
	'pfunc_time_error'                      => 'ত্রুটি: অবৈধ সময়',
	'pfunc_time_too_long'                   => 'ত্রুটি: অত্যধিক সংখ্যক #time কল',
	'pfunc_rel2abs_invalid_depth'           => 'ত্রুটি: পাথে অবৈধ গভীরতা: "$1" (মূল নোডের উপরের একটি নোড অ্যাক্সেস করতে চেষ্টা করেছিল)',
	'pfunc_expr_stack_exhausted'            => 'এক্সপ্রেশন ত্রুটি: স্ট্যাক শেষ হয়ে গেছে',
	'pfunc_expr_unexpected_number'          => 'এক্সপ্রেশন ত্রুটি: অযাচিত সংখ্যা',
	'pfunc_expr_preg_match_failure'         => 'এক্সপ্রেশন ত্রুটি: অযাচিত preg_match ব্যর্থতা',
	'pfunc_expr_unrecognised_word'          => 'এক্সপ্রেশন ত্রুটি: অপরিচিত শব্দ "$1"',
	'pfunc_expr_unexpected_operator'        => 'এক্সপ্রেশন ত্রুটি: অযাচিত $1 অপারেটর',
	'pfunc_expr_missing_operand'            => 'এক্সপ্রেশন ত্রুটি: $1-এর জন্য অপারেন্ড নেই।',
	'pfunc_expr_unexpected_closing_bracket' => 'এক্সপ্রেশন ত্রুটি: অযাচিত সমাপ্তকারী বন্ধনী',
	'pfunc_expr_unrecognised_punctuation'   => 'এক্সপ্রেশন ত্রুটি: অপরিচিত বিরামচিহ্ন ক্যারেক্টার "$1"',
	'pfunc_expr_unclosed_bracket'           => 'এক্সপ্রেশন ত্রুটি: উন্মুক্ত বন্ধনী',
	'pfunc_expr_division_by_zero'           => 'শূন্য দ্বারা ভাগ করা হয়েছে',
	'pfunc_expr_unknown_error'              => 'এক্সপ্রেশন ত্রুটি: অজানা ত্রুটি ($1)',
	'pfunc_expr_not_a_number'               => '$1: এ ফলাফল কোন সংখ্যা নয়',
	'pfunc_ifexist_warning'                 => 'সতর্কিকরণ: এই পাতায় খুব বেশি ifexist ডাকা(call) করা হয়েছে। যার সংখ্যা $2 এর হওয়া উচিত ছিল, এখন আছে $1।',
	'pfunc_max_ifexist_category'            => 'খুব বেশি ifexist  ডাকা(call) করা হয়েছে এমন পাতাসমূহ',
);

/** Catalan (Català)
 * @author SMP
 */
$messages['ca'] = array(
	'pfunc_time_error'                      => 'Error: temps invàlid',
	'pfunc_time_too_long'                   => 'Error: massa crides #time',
	'pfunc_rel2abs_invalid_depth'           => "Error: Adreça invàlida al directori: «$1» (s'intentava accedir a un node superior de l'arrel)",
	'pfunc_expr_stack_exhausted'            => "Error de l'expressió: Pila exhaurida",
	'pfunc_expr_unexpected_number'          => "Error de l'expressió: Nombre inesperat",
	'pfunc_expr_preg_match_failure'         => "Error de l'expressió: Error de funció no compresa i inesperada",
	'pfunc_expr_unrecognised_word'          => 'Error de l\'expressió: Paraula no reconeguda "$1"',
	'pfunc_expr_unexpected_operator'        => "Error de l'expressió: Operador $1 inesperat",
	'pfunc_expr_missing_operand'            => "Error de l'expressió: Falta l'operand de $1",
	'pfunc_expr_unexpected_closing_bracket' => "Error de l'expressió: Parèntesi inesperat",
	'pfunc_expr_unrecognised_punctuation'   => 'Error de l\'expressió: Signe de puntuació no reconegut "$1"',
	'pfunc_expr_unclosed_bracket'           => "Error de l'expressió: Parèntesi no tancat",
	'pfunc_expr_division_by_zero'           => 'Divisió entre zero',
	'pfunc_expr_unknown_error'              => "Error de l'expressió: Desconegut ($1)",
	'pfunc_expr_not_a_number'               => 'A $1: el resultat no és un nombre',
	'pfunc_ifexist_warning'                 => "Alerta: Aquesta pàgina conté massa crides #ifexist. N'hi hauria d'haver menys de $2 mentre que ara n'hi ha $1.",
	'pfunc_max_ifexist_category'            => 'Pàgines amb massa crides ifexist',
);

/** Czech (Česky)
 * @author Li-sung
 * @author Danny B.
 * @author Siebrand
 * @author Matěj Grabovský
 */
$messages['cs'] = array(
	'pfunc_desc'                            => 'Rozšíření syntaktického analyzátoru o logické funkce',
	'pfunc_time_error'                      => 'Chyba: neplatný čas',
	'pfunc_time_too_long'                   => 'Chyba: příliš mnoho volání #time',
	'pfunc_rel2abs_invalid_depth'           => 'Chyba: Neplatná hloubka v cestě: "$1" (pokus o přístup do uzlu vyššího než kořen)',
	'pfunc_expr_stack_exhausted'            => 'Chyba ve výrazu: Zásobník plně obsazen',
	'pfunc_expr_unexpected_number'          => 'Chyba ve výrazu: Očekáváno číslo',
	'pfunc_expr_preg_match_failure'         => 'Chyba ve výrazu: Neočekávaná chyba funkce preg_match',
	'pfunc_expr_unrecognised_word'          => 'Chyba ve výrazu: Nerozpoznané slovo „$1“',
	'pfunc_expr_unexpected_operator'        => 'Chyba ve výrazu: Neočekávaný operátor $1',
	'pfunc_expr_missing_operand'            => 'Chyba ve výrazu: Chybí operand pro $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Chyba ve výrazu: Neočekávaná uzavírací závorka',
	'pfunc_expr_unrecognised_punctuation'   => 'Chyba ve výrazu: Nerozpoznaný interpunkční znak „$1“',
	'pfunc_expr_unclosed_bracket'           => 'Chyba ve výrazu: Neuzavřené závorky',
	'pfunc_expr_division_by_zero'           => 'Dělení nulou',
	'pfunc_expr_unknown_error'              => 'Chyba ve výrazu: Neznámá chyba ($1)',
	'pfunc_expr_not_a_number'               => 'V $1: výsledkem není číslo',
	'pfunc_ifexist_warning'                 => 'Varování: Tato stránka obsahuje příliš mnoho volání funkce #ifexist. Mělo by jich být méně než $2, momentálně jich je $1.',
	'pfunc_max_ifexist_category'            => 'Stránky s příliš mnoha voláními funkce ifexist',
);

/**  German (Deutsch)
 * @author Raimond Spekking
 */
$messages['de'] = array(
	'pfunc_desc'                            => 'Erweitert den Parser um logische Funktionen',
	'pfunc_time_error'                      => 'Fehler: ungültige Zeitangabe',
	'pfunc_time_too_long'                   => 'Fehler: zu viele #time-Aufrufe',
	'pfunc_rel2abs_invalid_depth'           => 'Fehler: ungültige Tiefe in Pfad: „$1“ (Versuch, auf einen Knotenpunkt oberhalb des Hauptknotenpunktes zuzugreifen)',
	'pfunc_expr_stack_exhausted'            => 'Expression-Fehler: Stacküberlauf',
	'pfunc_expr_unexpected_number'          => 'Expression-Fehler: Unerwartete Zahl',
	'pfunc_expr_preg_match_failure'         => 'Expression-Fehler: Unerwartete „preg_match“-Fehlfunktion',
	'pfunc_expr_unrecognised_word'          => 'Expression-Fehler: Unerkanntes Wort „$1“',
	'pfunc_expr_unexpected_operator'        => 'Expression-Fehler: Unerwarteter Operator: <tt>$1</tt>',
	'pfunc_expr_missing_operand'            => 'Expression-Fehler: Fehlender Operand für <tt>$1</tt>',
	'pfunc_expr_unexpected_closing_bracket' => 'Expression-Fehler: Unerwartete schließende eckige Klammer',
	'pfunc_expr_unrecognised_punctuation'   => 'Expression-Fehler: Unerkanntes Satzzeichen „$1“',
	'pfunc_expr_unclosed_bracket'           => 'Expression-Fehler: Nicht geschlossene eckige Klammer',
	'pfunc_expr_division_by_zero'           => 'Expression-Fehler: Division durch Null',
	'pfunc_expr_unknown_error'              => 'Expression-Fehler: Unbekannter Fehler ($1)',
	'pfunc_expr_not_a_number'               => 'Expression-Fehler: In $1: Ergebnis ist keine Zahl',
	'pfunc_ifexist_warning'                 => 'Warnung: Diese Seite enthält zuviele #ifexist-Aufrufe. Es dürfen maximal $2 Aufrufe sein, es sind aber $1 Aufrufe.',
	'pfunc_max_ifexist_category'            => 'Seiten mit zuvielen ifexist-Aufrufen',
);

/** Greek (Ελληνικά)
 * @author Απεργός
 * @author Consta
 */
$messages['el'] = array(
	'pfunc_time_error'           => 'Σφάλμα: άκυρος χρόνος',
	'pfunc_time_too_long'        => 'Σφάλμα: πάρα πολλές κλήσεις της #time',
	'pfunc_ifexist_warning'      => 'Προειδοποίηση: Αυτή η σελίδα περιέχει πάρα πολλές κλήσεις της #ifexist.  Θα έπρεπε να έχει λιγότερες από $2, καθώς τώρα έχει $1.',
	'pfunc_max_ifexist_category' => 'Σελίδες με πάρα πολλές κλήσεις της ifexist',
);

/** فارسی (فارسی)
 * @author Huji
 */
$messages['fa'] = array(
	'pfunc_desc'                            => 'به تجزیه‌کننده، دستورهای منطقی می‌افزاید',
	'pfunc_time_error'                      => 'خطا: زمان غیرمجاز',
	'pfunc_time_too_long'                   => 'خطا: فراخوانی بیش از حد #time',
	'pfunc_rel2abs_invalid_depth'           => 'خطا: عمق غیر مجاز در نشانی «$1» (تلاش برای دسترسی به یک نشانی فراتر از نشانی ریشه)',
	'pfunc_expr_stack_exhausted'            => 'خطای عبارت: پشته از دست رفته',
	'pfunc_expr_unexpected_number'          => 'خطای عبارت: عدد دور از انتظار',
	'pfunc_expr_preg_match_failure'         => 'خطای عبارت: خطای preg_match دور از انتظار',
	'pfunc_expr_unrecognised_word'          => 'خطای عبارت: کلمه ناشناخته «$1»',
	'pfunc_expr_unexpected_operator'        => 'خطای عبارت: عملگر $1 دور از انتظار',
	'pfunc_expr_missing_operand'            => 'خطای عبارت: عملگر گمشده برای $1',
	'pfunc_expr_unexpected_closing_bracket' => 'خطای عبارت: پرانتز بسته اضافی',
	'pfunc_expr_unrecognised_punctuation'   => 'خطای عبارت: نویسه نقطه‌گذاری شناخته نشده «$1»',
	'pfunc_expr_unclosed_bracket'           => 'خطای عبارت: پرانتز بسته‌نشده',
	'pfunc_expr_division_by_zero'           => 'تقسیم بر صفر',
	'pfunc_expr_unknown_error'              => 'خطای عبارت: خطای ناشناخته ($1)',
	'pfunc_expr_not_a_number'               => 'در $1: نتیجه عدد نیست',
	'pfunc_ifexist_warning'                 => 'هشدار: این صفحه حاوی فراخوانی‌های بیش از حد ifexist است. حداکثر تعداد فراخوانی مجاز $2 است، تعداد آن در این صفحه $1 است.',
	'pfunc_max_ifexist_category'            => 'صفحه‌های دارای فراخوانی بیش از حد ifexist',

);

/** French (Français)
 * @author Grondin
 * @author Sherbrooke
 * @author Urhixidur
 */
$messages['fr'] = array(
	'pfunc_desc'                            => 'Augmente le parseur avec des fonctions logiques',
	'pfunc_time_error'                      => 'Erreur : durée invalide',
	'pfunc_time_too_long'                   => 'Erreur : <code>#time</code> appelé trop de fois',
	'pfunc_rel2abs_invalid_depth'           => 'Erreur: niveau de répertoire invalide dans le chemin : « $1 » (a essayé d’accéder à un niveau au-dessus du répertoire racine)',
	'pfunc_expr_stack_exhausted'            => 'Expression erronée : pile épuisée',
	'pfunc_expr_unexpected_number'          => 'Expression erronée : nombre inattendu',
	'pfunc_expr_preg_match_failure'         => 'Expression erronée : échec inattendu pour <code>preg_match</code>',
	'pfunc_expr_unrecognised_word'          => "Erreur d'expression : le mot '''$1''' n'est pas reconnu",
	'pfunc_expr_unexpected_operator'        => "Erreur d'expression : l'opérateur '''$1''' n'est pas reconnu",
	'pfunc_expr_missing_operand'            => "Erreur d’expression : l’opérande '''$1''' n’est pas reconnue",
	'pfunc_expr_unexpected_closing_bracket' => 'Erreur d’expression : parenthèse fermante inattendue',
	'pfunc_expr_unrecognised_punctuation'   => "Erreur d'expression : caractère de ponctuation « $1 » non reconnu",
	'pfunc_expr_unclosed_bracket'           => 'Erreur d’expression : parenthèse non fermée',
	'pfunc_expr_division_by_zero'           => 'Division par zéro',
	'pfunc_expr_unknown_error'              => "Erreur d'expression : erreur inconnue ($1)",
	'pfunc_expr_not_a_number'               => "Dans $1 : le résultat n'est pas un nombre",
	'pfunc_ifexist_warning'                 => "Attention : Cette page contient trop d'appels à <code>#ifexist</code>. Elle devrait en avoir moins que $2, alors qu'elle en a $1.",
	'pfunc_max_ifexist_category'            => "Pages avec trop d'appels à <code>#ifexist</code>",
);

/** Franco-Provençal (Arpetan)
 * @author ChrisPtDe
 */
$messages['frp'] = array(
	'pfunc_desc'                            => 'Ôgmente lo parsor avouéc des fonccions logiques.',
	'pfunc_time_error'                      => 'Èrror : durâ envalida',
	'pfunc_time_too_long'                   => 'Èrror : parsèr #time apelâ trop de côps',
	'pfunc_rel2abs_invalid_depth'           => 'Èrror : nivô de rèpèrtouèro envalido dens lo chemin : « $1 » (at tâchiê d’arrevar a un nivô en-dessus du rèpèrtouèro racena)',
	'pfunc_expr_stack_exhausted'            => 'Èxprèssion fôssa : pila èpouesiê',
	'pfunc_expr_unexpected_number'          => 'Èxprèssion fôssa : nombro emprèvu',
	'pfunc_expr_preg_match_failure'         => 'Èxprèssion fôssa : falyita emprèvua por <code>preg_match</code>',
	'pfunc_expr_unrecognised_word'          => "Èrror d’èxprèssion : lo mot '''$1''' est pas recognu",
	'pfunc_expr_unexpected_operator'        => "Èrror d’èxprèssion : l’opèrator '''$1''' est pas recognu",
	'pfunc_expr_missing_operand'            => "Èrror d’èxprèssion : l’opèranda '''$1''' est pas recognua",
	'pfunc_expr_unexpected_closing_bracket' => 'Èrror d’èxprèssion : parentèsa cllosenta emprèvua',
	'pfunc_expr_unrecognised_punctuation'   => 'Èrror d’èxprèssion : caractèro de ponctuacion « $1 » pas recognu',
	'pfunc_expr_unclosed_bracket'           => 'Èrror d’èxprèssion : parentèsa pas cllôsa',
	'pfunc_expr_division_by_zero'           => 'Division per zérô',
	'pfunc_expr_unknown_error'              => 'Èrror d’èxprèssion : èrror encognua ($1)',
	'pfunc_expr_not_a_number'               => 'Dens $1 : lo rèsultat est pas un nombro',
	'pfunc_ifexist_warning'                 => 'Atencion : ceta pâge contint trop d’apèls a <code>#ifexist</code>. Devrêt nen avêr muens que $2, pendent qu’el en at $1.',
	'pfunc_max_ifexist_category'            => 'Pâges avouéc trop d’apèls a <code>#ifexist</code>',
);

/** Galician (Galego)
 * @author Xosé
 * @author Alma
 * @author Siebrand
 */
$messages['gl'] = array(
	'pfunc_time_error'                      => 'Erro: hora non válida',
	'pfunc_time_too_long'                   => 'Erro: demasiadas chamadas a #time',
	'pfunc_rel2abs_invalid_depth'           => 'Erro: Profundidade da ruta non válida: "$1" (tentouse acceder a un nodo por riba do nodo raíz)',
	'pfunc_expr_stack_exhausted'            => 'Erro de expresión: Pila esgotada',
	'pfunc_expr_unexpected_number'          => 'Erro de expresión: Número inesperado',
	'pfunc_expr_preg_match_failure'         => 'Erro de expresión: Fallo de preg_match inesperado',
	'pfunc_expr_unrecognised_word'          => 'Erro de expresión: Palabra descoñecida "$1"',
	'pfunc_expr_unexpected_operator'        => 'Erro de expresión: Operador $1 inesperado',
	'pfunc_expr_missing_operand'            => 'Erro de expresión: Falta un operador para $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Expresión de erro: Inesperado corchete',
	'pfunc_expr_unrecognised_punctuation'   => 'Erro de expresión: Signo de puntuación descoñecido "$1"',
	'pfunc_expr_unclosed_bracket'           => 'Erro de expresión: Paréntese sen pechar',
	'pfunc_expr_division_by_zero'           => 'División por cero',
	'pfunc_expr_unknown_error'              => 'Erro de expresión: Erro descoñecido ($1)',
	'pfunc_expr_not_a_number'               => 'En $1: o resultado non é un número',
	'pfunc_ifexist_warning'                 => 'Advertencia: Este artigo inclúe demasiadas chamadas a #ifexists. Debería ter menos de $2 e agora hai $1.',
	'pfunc_max_ifexist_category'            => 'Páxinas con demasiadas chamadas a ifexists',
);

/** Hebrew (עברית) */
$messages['he'] = array(
	'pfunc_time_error'                      => 'שגיאה: זמן שגוי',
	'pfunc_time_too_long'                   => 'שגיאה: שימוש ב"#זמן" פעמים רבות מדי',
	'pfunc_rel2abs_invalid_depth'           => 'שגיאה: עומק שגוי בנתיב: "$1" (ניסיון כניסה לצומת מעל צומת השורש)',
	'pfunc_expr_stack_exhausted'            => 'שגיאה בביטוי: המחסנית מלאה',
	'pfunc_expr_unexpected_number'          => 'שגיאה בביטוי: מספר בלתי צפוי',
	'pfunc_expr_preg_match_failure'         => 'שגיאה בביטוי: כישלון בלתי צפוי של התאמת ביטוי רגולרי',
	'pfunc_expr_unrecognised_word'          => 'שגיאה בביטוי: מילה בלתי מזוהה, "$1"',
	'pfunc_expr_unexpected_operator'        => 'שגיאה בביטוי: אופרנד $1 בלתי צפוי',
	'pfunc_expr_missing_operand'            => 'שגיאה בביטוי: חסר אופרנד ל־$1',
	'pfunc_expr_unexpected_closing_bracket' => 'שגיאה בביטוי: סוגריים סוגרים בלתי צפויים',
	'pfunc_expr_unrecognised_punctuation'   => 'שגיאה בביטוי: תו פיסוק בלתי מזוהה, "$1"',
	'pfunc_expr_unclosed_bracket'           => 'שגיאה בביטוי: סוגריים בלתי סגורים',
	'pfunc_expr_division_by_zero'           => 'חלוקה באפס',
	'pfunc_expr_unknown_error'              => 'שגיאה בביטוי: שגיאה בלתי ידועה ($1)',
	'pfunc_expr_not_a_number'               => 'התוצאה של $1 אינה מספר',
	'pfunc_ifexist_warning'                 => 'אזהרה: דף זה כולל יותר מדי קריאות ל"#קיים". הוא צריך לכלול פחות מ־$2, אך כעת יש $1.',
	'pfunc_max_ifexist_category'            => 'דפים עם הוראות קיום רבות מדי',
);

/** Croatian (Hrvatski)
 * @author SpeedyGonsales
 * @author Siebrand
 * @author Dnik
 */
$messages['hr'] = array(
	'pfunc_desc'                            => 'Proširite parser logičkim funkcijama',
	'pfunc_time_error'                      => 'Greška: oblik vremena nije valjan',
	'pfunc_time_too_long'                   => 'Greška: prevelik broj #time (vremenskih) poziva',
	'pfunc_rel2abs_invalid_depth'           => 'Greška: Nevaljana dubina putanje: "$1" (pokušaj pristupanja čvoru iznad korijenskog)',
	'pfunc_expr_stack_exhausted'            => 'Greška u predlošku: prepunjen stog',
	'pfunc_expr_unexpected_number'          => 'Greška u predlošku: Neočekivan broj',
	'pfunc_expr_preg_match_failure'         => 'Greška u predlošku: Neočekivana preg_match greška',
	'pfunc_expr_unrecognised_word'          => 'Greška u predlošku: Nepoznata riječ "$1"',
	'pfunc_expr_unexpected_operator'        => 'Greška u predlošku: Neočekivani operator $1',
	'pfunc_expr_missing_operand'            => 'Greška u predlošku: Operator $1 nedostaje',
	'pfunc_expr_unexpected_closing_bracket' => 'Greška u predlošku: Neočekivana zatvorena zagrada',
	'pfunc_expr_unrecognised_punctuation'   => 'Greška u predlošku: Nepoznat interpunkcijski znak "$1"',
	'pfunc_expr_unclosed_bracket'           => 'Greška u predlošku: Nezatvorene zagrade',
	'pfunc_expr_division_by_zero'           => 'Dijeljenje s nulom',
	'pfunc_expr_unknown_error'              => 'Greška u predlošku: Nepoznata greška ($1)',
	'pfunc_expr_not_a_number'               => 'U $1: rezultat nije broj',
	'pfunc_ifexist_warning'                 => 'Upozorenje: Ova stranica sadrži previše #ifexist poziva. Treba ih biti manje od $2, trenutno ih je $1.',
	'pfunc_max_ifexist_category'            => 'Stranica s previše ifexist poziva',
);

/** Upper Sorbian (Hornjoserbsce)
 * @author Michawiki
 */
$messages['hsb'] = array(
	'pfunc_desc'                            => 'Parser wo logiske funkcije rozšěrić',
	'pfunc_time_error'                      => 'Zmylk: njepłaćiwe časowe podaće',
	'pfunc_time_too_long'                   => 'Zmylk: přewjele zawołanjow #time',
	'pfunc_rel2abs_invalid_depth'           => 'Zmylk: Njepłaćiwa hłubokosć w pućiku: "$1" (Pospyt, zo by na suk wyše hłowneho suka dohrabnyło)',
	'pfunc_expr_stack_exhausted'            => 'Wurazowy zmylk: Staplowy skład wučerpany',
	'pfunc_expr_unexpected_number'          => 'Wurazowy zmylk: Njewočakowana ličba',
	'pfunc_expr_preg_match_failure'         => 'Wurazowy zmylk: Njewočakowana zmylna funkcija "preg_match"',
	'pfunc_expr_unrecognised_word'          => 'Wurazowy zmylk: Njespóznate słowo "$1"',
	'pfunc_expr_unexpected_operator'        => 'Wurazowy zmylk: Njewočakowany operator $1',
	'pfunc_expr_missing_operand'            => 'Wurazowy zmylk: Falowacy operand za $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Wurazowy zmylk: Njewočakowana kónčna róžkata spinka',
	'pfunc_expr_unrecognised_punctuation'   => 'Wurazowy zmylk: Njespóznate interpunkciske znamješko "$1"',
	'pfunc_expr_unclosed_bracket'           => 'Wurazowy zmylk: Njewotzamknjena róžkata spinka',
	'pfunc_expr_division_by_zero'           => 'Diwizija přez nulu',
	'pfunc_expr_unknown_error'              => 'Wurazowy zmylk: Njeznaty zmylk ($1)',
	'pfunc_expr_not_a_number'               => 'W $1: Wuslědk ličba njeje',
	'pfunc_ifexist_warning'                 => 'Warnowanje: Tutón nastawk wobsahuje přewjele zawołanjow #ifexist. Měło mjenje hač $2 być, nětko je $1.',
	'pfunc_max_ifexist_category'            => 'Strony z přewjele zawołanjow ifexist',
);

/** Hungarian (Magyar)
 * @author Bdanee
 */
$messages['hu'] = array(
	'pfunc_desc'                            => 'Az értelmező kiegészítése logikai funkciókkal',
	'pfunc_time_error'                      => 'Hiba: érvénytelen idő',
	'pfunc_time_too_long'                   => 'Hiba: a #time túl sokszor lett meghívva',
	'pfunc_rel2abs_invalid_depth'           => 'Hiba: nem megfelelő a mélység az elérési útban: „$1” (egy olyan csomópontot akartál elérni, amely a gyökércsomópont felett van)',
	'pfunc_expr_stack_exhausted'            => 'Hiba a kifejezésben: a verem kiürült',
	'pfunc_expr_unexpected_number'          => 'Hiba a kifejezésben: nem várt szám',
	'pfunc_expr_preg_match_failure'         => 'Hiba a kifejezésben: a preg_match váratlanul hibát jelzett',
	'pfunc_expr_unrecognised_word'          => 'Hiba a kifejezésben: ismeretlen „$1” szó',
	'pfunc_expr_unexpected_operator'        => 'Hiba a kifejezésben: nem várt $1 operátor',
	'pfunc_expr_missing_operand'            => 'Hiba a kifejezésben: $1 egyik operandusa hiányzik',
	'pfunc_expr_unexpected_closing_bracket' => 'Hiba a kifejezésben: nem várt zárójel',
	'pfunc_expr_unrecognised_punctuation'   => 'Hiba a kifejezésben: ismeretlen „$1” központozó karakter',
	'pfunc_expr_unclosed_bracket'           => 'Hiba a kifejezésben: lezáratlan zárójel',
	'pfunc_expr_division_by_zero'           => 'Nullával való osztás',
	'pfunc_expr_unknown_error'              => 'Hiba a kifejezésben: ismeretlen hiba ($1)',
	'pfunc_expr_not_a_number'               => '$1: az eredmény nem szám',
	'pfunc_ifexist_warning'                 => 'Figyelem: az oldal túl sok #ifexist hívást tartalmaz. Kevesebb, mint $2 darabnak kellene lennie, most $1 van.',
	'pfunc_max_ifexist_category'            => 'Túl sok ifexist hívást tartalmazó lapok',
);

/** Italian (Italiano)
 * @author BrokenArrow
 */
$messages['it'] = array(
	'pfunc_desc'                            => 'Aggiunge al parser una serie di funzioni logiche',
	'pfunc_time_error'                      => 'Errore: orario non valido',
	'pfunc_time_too_long'                   => 'Errore: troppe chiamate a #time',
	'pfunc_rel2abs_invalid_depth'           => 'Errore: profondità non valida nel percorso "$2" (si è tentato di accedere a un nodo superiore alla radice)',
	'pfunc_expr_stack_exhausted'            => "Errore nell'espressione: stack esaurito",
	'pfunc_expr_unexpected_number'          => "Errore nell'espressione: numero inatteso",
	'pfunc_expr_preg_match_failure'         => "Errore nell'espressione: errore inatteso in preg_match",
	'pfunc_expr_unrecognised_word'          => 'Errore nell\'espressione: parola "$1" non riconosciuta',
	'pfunc_expr_unexpected_operator'        => "Errore nell'espressione: operatore $1 inatteso",
	'pfunc_expr_missing_operand'            => "Errore nell'espressione: operando mancante per $1",
	'pfunc_expr_unexpected_closing_bracket' => "Errore nell'espressione: parentesi chiusa inattesa",
	'pfunc_expr_unrecognised_punctuation'   => 'Errore nell\'espressione: carattere di punteggiatura "$1" non riconosciuto',
	'pfunc_expr_unclosed_bracket'           => "Errore nell'espressione: parentesi non chiusa",
	'pfunc_expr_division_by_zero'           => 'Divisione per zero',
	'pfunc_expr_unknown_error'              => "Errore nell'espressione: errore sconosciuto ($1)",
	'pfunc_expr_not_a_number'               => 'In $1: il risultato non è un numero',
	'pfunc_ifexist_warning'                 => 'Attenzione: in questa pagina sono presenti troppe chiamate alla funzione #ifexist. Il numero massimo è $2, al momento ve ne sono $1.',
	'pfunc_max_ifexist_category'            => 'Pagine con troppe chiamate alla funzione ifexist',
);

/** Japanese (日本語)
 * @author JtFuruhata
 */
$messages['ja'] = array(
	'pfunc_desc'                            => '論理関数によるパーサー拡張',
	'pfunc_time_error'                      => 'エラー: 時刻が不正です',
	'pfunc_time_too_long'                   => 'エラー: #time 呼び出しが多すぎます',
	'pfunc_rel2abs_invalid_depth'           => 'エラー: パス "$1" の階層が不正です（ルート階層からのアクセスをお試しください）',
	'pfunc_expr_stack_exhausted'            => '構文エラー: スタックが空です',
	'pfunc_expr_unexpected_number'          => '構文エラー: 予期せぬ数字です',
	'pfunc_expr_preg_match_failure'         => '構文エラー: 予期せぬ形で preg_match に失敗しました',
	'pfunc_expr_unrecognised_word'          => '構文エラー: "$1" は認識できません',
	'pfunc_expr_unexpected_operator'        => '構文エラー: 予期せぬ演算子 $1 があります',
	'pfunc_expr_missing_operand'            => '構文エラー: $1 のオペランドがありません',
	'pfunc_expr_unexpected_closing_bracket' => '構文エラー: 予期せぬ閉じ括弧です',
	'pfunc_expr_unrecognised_punctuation'   => '構文エラー: 認識できない区切り文字 "$1" があります',
	'pfunc_expr_unclosed_bracket'           => '構文エラー: 括弧が閉じられていません',
	'pfunc_expr_division_by_zero'           => '0で除算しました',
	'pfunc_expr_unknown_error'              => '構文エラー: 予期せぬエラー（$1）',
	'pfunc_expr_not_a_number'               => '$1: 結果が数字ではありません',
	'pfunc_ifexist_warning'                 => '警告: このページには多数の #ifexist 呼び出しが含まれています。これは$2個未満でなければならず、現在は$1個記述されています。',
	'pfunc_max_ifexist_category'            => 'このページは #ifexist 呼び出しが多すぎます',
);

/** ‫قازاقشا (تٴوتە)‬ (‫قازاقشا (تٴوتە)‬) */
$messages['kk-arab'] = array(
	'pfunc_time_error'                      => 'قاتە: جارامسىز ۋاقىت',
	'pfunc_time_too_long'                   => 'قاتە: #time شاقىرۋى تىم كوپ',
	'pfunc_rel2abs_invalid_depth'           => 'قاتە: مىنا جولدىڭ جارامسىز تەرەندىگى «$1» (تامىر ٴتۇيىننىڭ ۇستىندەگى تۇيىنگە قاتىناۋ تالابى)',
	'pfunc_expr_stack_exhausted'            => 'ايتىلىم قاتەسى: ستەك سارقىلدى',
	'pfunc_expr_unexpected_number'          => 'ايتىلىم قاتەسى: كۇتىلمەگەن سان',
	'pfunc_expr_preg_match_failure'         => 'ايتىلىم قاتەسى: كۇتىلمەگەن preg_match ساتسىزدىگى',
	'pfunc_expr_unrecognised_word'          => 'ايتىلىم قاتەسى: تانىلماعان ٴسوز «$1»',
	'pfunc_expr_unexpected_operator'        => 'ايتىلىم قاتەسى: كۇتىلمەگەن وپەراتور $1',
	'pfunc_expr_missing_operand'            => 'ايتىلىم قاتەسى: $1 ٴۇشىن جوعالعان وپەراند ',
	'pfunc_expr_unexpected_closing_bracket' => 'ايتىلىم قاتەسى: كۇتىلمەگەن جاباتىن جاقشا',
	'pfunc_expr_unrecognised_punctuation'   => 'ايتىلىم قاتەسى: تانىلماعان تىنىس بەلگىسى «$1» ',
	'pfunc_expr_unclosed_bracket'           => 'ايتىلىم قاتەسى: جابىلماعان جاقشا',
	'pfunc_expr_division_by_zero'           => 'نولگە ٴبولىنۋى',
	'pfunc_expr_unknown_error'              => 'ايتىلىم قاتەسى: بەلگىسىز قاتە ($1)',
	'pfunc_expr_not_a_number'               => '$1 دەگەندە: ناتىيجە سان ەمەس',
	'pfunc_ifexist_warning'                 => 'نازار سالىڭىز: وسى بەتتە تىم كوپ #ifexist شاقىرىمدارى بار. بۇل $2 ساننان كەم بولۋى كەرەك, مىندا قازىر $1 بار.',
	'pfunc_max_ifexist_category'            => 'تىم كوپ ifexist شاقىرىمدارى بار بەتتەر',
);

/** Kazakh (Cyrillic) (Қазақша (Cyrillic)) */
$messages['kk-cyrl'] = array(
	'pfunc_time_error'                      => 'Қате: жарамсыз уақыт',
	'pfunc_time_too_long'                   => 'Қате: #time шақыруы тым көп',
	'pfunc_rel2abs_invalid_depth'           => 'Қате: Мына жолдың жарамсыз терендігі «$1» (тамыр түйіннің үстіндегі түйінге қатынау талабы)',
	'pfunc_expr_stack_exhausted'            => 'Айтылым қатесі: Стек сарқылды',
	'pfunc_expr_unexpected_number'          => 'Айтылым қатесі: Күтілмеген сан',
	'pfunc_expr_preg_match_failure'         => 'Айтылым қатесі: Күтілмеген preg_match сәтсіздігі',
	'pfunc_expr_unrecognised_word'          => 'Айтылым қатесі: Танылмаған сөз «$1»',
	'pfunc_expr_unexpected_operator'        => 'Айтылым қатесі: Күтілмеген оператор $1',
	'pfunc_expr_missing_operand'            => 'Айтылым қатесі: $1 үшін жоғалған операнд ',
	'pfunc_expr_unexpected_closing_bracket' => 'Айтылым қатесі: Күтілмеген жабатын жақша',
	'pfunc_expr_unrecognised_punctuation'   => 'Айтылым қатесі: Танылмаған тыныс белгісі «$1» ',
	'pfunc_expr_unclosed_bracket'           => 'Айтылым қатесі: Жабылмаған жақша',
	'pfunc_expr_division_by_zero'           => 'Нөлге бөлінуі',
	'pfunc_expr_unknown_error'              => 'Айтылым қатесі: Белгісіз қате ($1)',
	'pfunc_expr_not_a_number'               => '$1 дегенде: нәтиже сан емес',
	'pfunc_ifexist_warning'                 => 'Назар салыңыз: Осы бетте тым көп #ifexist шақырымдары бар. Бұл $2 саннан кем болуы керек, мында қазір $1 бар.',
	'pfunc_max_ifexist_category'            => 'Тым көп ifexist шақырымдары бар беттер',
);

/** Kazakh (Latin) (Қазақша (Latin)) */
$messages['kk-latn'] = array(
	'pfunc_time_error'                      => 'Qate: jaramsız waqıt',
	'pfunc_time_too_long'                   => 'Qate: #time şaqırwı tım köp',
	'pfunc_rel2abs_invalid_depth'           => 'Qate: Mına joldıñ jaramsız terendigi «$1» (tamır tüýinniñ üstindegi tüýinge qatınaw talabı)',
	'pfunc_expr_stack_exhausted'            => 'Aýtılım qatesi: Stek sarqıldı',
	'pfunc_expr_unexpected_number'          => 'Aýtılım qatesi: Kütilmegen san',
	'pfunc_expr_preg_match_failure'         => 'Aýtılım qatesi: Kütilmegen preg_match sätsizdigi',
	'pfunc_expr_unrecognised_word'          => 'Aýtılım qatesi: Tanılmağan söz «$1»',
	'pfunc_expr_unexpected_operator'        => 'Aýtılım qatesi: Kütilmegen operator $1',
	'pfunc_expr_missing_operand'            => 'Aýtılım qatesi: $1 üşin joğalğan operand ',
	'pfunc_expr_unexpected_closing_bracket' => 'Aýtılım qatesi: Kütilmegen jabatın jaqşa',
	'pfunc_expr_unrecognised_punctuation'   => 'Aýtılım qatesi: Tanılmağan tınıs belgisi «$1» ',
	'pfunc_expr_unclosed_bracket'           => 'Aýtılım qatesi: Jabılmağan jaqşa',
	'pfunc_expr_division_by_zero'           => 'Nölge bölinwi',
	'pfunc_expr_unknown_error'              => 'Aýtılım qatesi: Belgisiz qate ($1)',
	'pfunc_expr_not_a_number'               => '$1 degende: nätïje san emes',
	'pfunc_ifexist_warning'                 => 'Nazar salıñız: Osı bette tım köp #ifexist şaqırımdarı bar. Bul $2 sannan kem bolwı kerek, mında qazir $1 bar.',
	'pfunc_max_ifexist_category'            => 'Tım köp ifexist şaqırımdarı bar better',
);

/** Latin (Latina)
 * @author UV
 */
$messages['la'] = array(
	'pfunc_ifexist_warning'      => 'Monitio: Haec pagina nimis #ifexist adhibet. Licet uti $2, haec pagina nunc utitur $1.',
	'pfunc_max_ifexist_category' => 'Paginae quae nimis ifexist adhibent',
);

/** Luxembourgish (Lëtzebuergesch)
 * @author Robby
 */
$messages['lb'] = array(
	'pfunc_expr_division_by_zero' => 'Divisioun duerch Null',
	'pfunc_expr_not_a_number'     => "An $1: D'Resultat ass keng Zuel",
);

/** Limburgish (Limburgs)
 * @author Matthias
 * @author Ooswesthoesbes
 */
$messages['li'] = array(
	'pfunc_desc'                            => 'Verrijkt de parser met logische functies',
	'pfunc_time_error'                      => 'Fout: ongeldige tied',
	'pfunc_time_too_long'                   => 'Fout: #time te vaok aangerope',
	'pfunc_rel2abs_invalid_depth'           => 'Fout: ongeldige diepte in pad: "$1" (probeerde \'n node bove de stamnode aan te rope)',
	'pfunc_expr_stack_exhausted'            => 'Fout in oetdrukking: stack oetgeput',
	'pfunc_expr_unexpected_number'          => 'Fout in oetdrukking: onverwacht getal',
	'pfunc_expr_preg_match_failure'         => 'Fout in oetdrukking: onverwacht fale van preg_match',
	'pfunc_expr_unrecognised_word'          => 'Fout in oetdrukking: woord "$1" neet herkend',
	'pfunc_expr_unexpected_operator'        => 'Fout in oetdrukking: neet verwachte operator $1',
	'pfunc_expr_missing_operand'            => 'Fout in oetdrukking: operand veur $1 mist',
	'pfunc_expr_unexpected_closing_bracket' => 'Fout in oetdrukking: haakje sloete op onverwachte plaats',
	'pfunc_expr_unrecognised_punctuation'   => 'Fout in oetdrukking: neet herkend leesteke "$1"',
	'pfunc_expr_unclosed_bracket'           => 'Fout in oetdrukking: neet geslote haakje opene',
	'pfunc_expr_division_by_zero'           => 'Deiling door nul',
	'pfunc_expr_unknown_error'              => 'Fout in oetdrukking: ónbekindje fout ($1)',
	'pfunc_expr_not_a_number'               => 'In $1: rezultaot is gein getal',
	'pfunc_ifexist_warning'                 => "Waorsjuwing: dees pazjena gebroek #ifexists euverduk. Det zów minder es $2 kieër mótte zeen en 't is noe $1 kieër.",
	'pfunc_max_ifexist_category'            => "Pazjena's die iefexist euverduk gebroeke",
);

/** Low German (Plattdüütsch)
 * @author Slomox
 */
$messages['nds'] = array(
	'pfunc_time_error'                      => 'Fehler: mit de Tiet stimmt wat nich',
	'pfunc_time_too_long'                   => 'Fehler: #time warrt to faken opropen',
	'pfunc_rel2abs_invalid_depth'           => 'Fehler: Mit den Padd „$1“ stimmt wat nich, liggt nich ünner den Wuddelorner',
	'pfunc_expr_stack_exhausted'            => 'Fehler in’n Utdruck: Stack överlopen',
	'pfunc_expr_unexpected_number'          => 'Fehler in’n Utdruck: Unverwacht Tall',
	'pfunc_expr_preg_match_failure'         => 'Fehler in’n Utdruck: Unverwacht Fehler bi „preg_match“',
	'pfunc_expr_unrecognised_word'          => 'Fehler in’n Utdruck: Woort „$1“ nich kennt',
	'pfunc_expr_unexpected_operator'        => 'Fehler in’n Utdruck: Unverwacht Operator $1',
	'pfunc_expr_missing_operand'            => 'Fehler in’n Utdruck: Operand för $1 fehlt',
	'pfunc_expr_unexpected_closing_bracket' => 'Fehler in’n Utdruck: Unverwacht Klammer to',
	'pfunc_expr_unrecognised_punctuation'   => 'Fehler in’n Utdruck: Satzteken „$1“ nich kennt',
	'pfunc_expr_unclosed_bracket'           => 'Fehler in’n Utdruck: Nich slatene Klammer',
	'pfunc_expr_division_by_zero'           => 'Delen dör Null',
	'pfunc_expr_unknown_error'              => 'Fehler in’n Utdruck: Unbekannten Fehler ($1)',
	'pfunc_expr_not_a_number'               => 'In $1: wat rutkamen is, is kene Tall',
	'pfunc_ifexist_warning'                 => 'Wohrschau: Disse Siet bruukt #ifexist to faken. De Siet dröff nich mehr as $2 hebben, hett aver $1.',
	'pfunc_max_ifexist_category'            => 'Sieden, de #ifexist to faken bruukt',
);

/** Nepali (नेपाली)
 * @author SPQRobin
 */
$messages['ne'] = array(
	'pfunc_time_error'            => 'त्रुटी: गलत/वा हुदैनहुने समय',
	'pfunc_time_too_long'         => 'त्रुटी: एकदम धेरै #time callहरु',
	'pfunc_rel2abs_invalid_depth' => 'त्रुटी: पाथमा (इनभ्यालिड)गलत गहिराइ(डेप्थ) भयो: "$1" (ले रुट नोड भन्दापनि माथिको नोडलाइ चलाउन(एकसेस) गर्न खोज्यो)',
);

/** Dutch (Nederlands)
 * @author SPQRobin
 * @author Siebrand
 */
$messages['nl'] = array(
	'pfunc_desc'                            => 'Verrijkt de parser met logische functies',
	'pfunc_time_error'                      => 'Fout: ongeldige tijd',
	'pfunc_time_too_long'                   => 'Fout: #time te vaak aangeroepen',
	'pfunc_rel2abs_invalid_depth'           => 'Fout: ongeldige diepte in pad: "$1" (probeerde een node boven de stamnode aan te roepen)',
	'pfunc_expr_stack_exhausted'            => 'Fout in uitdrukking: stack uitgeput',
	'pfunc_expr_unexpected_number'          => 'Fout in uitdrukking: onverwacht getal',
	'pfunc_expr_preg_match_failure'         => 'Fout in uitdrukking: onverwacht falen van preg_match',
	'pfunc_expr_unrecognised_word'          => 'Fout in uitdrukking: woord "$1" niet herkend',
	'pfunc_expr_unexpected_operator'        => 'Fout in uitdrukking: niet verwachte operator $1',
	'pfunc_expr_missing_operand'            => 'Fout in uitdrukking: operand voor $1 mist',
	'pfunc_expr_unexpected_closing_bracket' => 'Fout in uitdrukking: haakje sluiten op onverwachte plaats',
	'pfunc_expr_unrecognised_punctuation'   => 'Fout in uitdrukking: niet herkend leesteken "$1"',
	'pfunc_expr_unclosed_bracket'           => 'Fout in uitdrukking: niet gesloten haakje openen',
	'pfunc_expr_division_by_zero'           => 'Deling door nul',
	'pfunc_expr_unknown_error'              => 'Fout in uitdrukking: onbekende fout ($1)',
	'pfunc_expr_not_a_number'               => 'In $1: resultaat is geen getal',
	'pfunc_ifexist_warning'                 => 'Waarschuwing: deze pagina gebruik #ifexists te vaak. Dat zou minder dan $2 keer moeten zijn en is nu $1 keer.',
	'pfunc_max_ifexist_category'            => "Pagina's die ifexist te vaak gebruiken",
);

/** Norwegian Nynorsk (‪Norsk (nynorsk)‬)
 * @author Eirik
 */
$messages['nn'] = array(
	'pfunc_desc'                            => 'Legg til logiske funksjonar i parseren.',
	'pfunc_time_error'                      => 'Feil: Ugyldig tid',
	'pfunc_time_too_long'                   => 'Feil: #time er kalla for mange gonger',
	'pfunc_rel2abs_invalid_depth'           => 'Feil: Ugyldig djupn i stien: «$1» (prøvde å nå ein node ovanfor rotnoden)',
	'pfunc_expr_stack_exhausted'            => 'Feil i uttrykket: Stacken er tømd',
	'pfunc_expr_unexpected_number'          => 'Feil i uttrykket: Uventa tal',
	'pfunc_expr_preg_match_failure'         => 'Feil i uttrykket: Uventa feil i preg_match',
	'pfunc_expr_unrecognised_word'          => 'Feil i uttrykket: Ukjent ord, «$1»',
	'pfunc_expr_unexpected_operator'        => 'Feil i uttrykket: Uventa operatør, $1',
	'pfunc_expr_missing_operand'            => 'Feil i uttrykket: Operand for $1 manglar',
	'pfunc_expr_unexpected_closing_bracket' => 'Feil i uttrykket: Uventa avsluttande parentes',
	'pfunc_expr_unrecognised_punctuation'   => 'Feil i uttrykket: Ukjent punktumsteikn, «$1»',
	'pfunc_expr_unclosed_bracket'           => 'Feil i uttrykket: Ein parentes er ikkje avslutta',
	'pfunc_expr_division_by_zero'           => 'Divisjon med null',
	'pfunc_expr_unknown_error'              => 'Feil i uttrykket: Ukjend feil ($1)',
	'pfunc_expr_not_a_number'               => 'Resultatet i $1 er ikkje eit tal',
	'pfunc_ifexist_warning'                 => 'Åtvaring: #ifexist er kalla for mange gonger på denne sida. Han kan ikkje vere kalla fleire gonger enn $2, men er no kalla $1 gonger.',
	'pfunc_max_ifexist_category'            => 'Sider med for mange kallingar av #ifexist',
);

/** Norwegian (‪Norsk (bokmål)‬)
 * @author Jon Harald Søby
 */
$messages['no'] = array(
	'pfunc_desc'                            => 'Utvid parser med logiske funksjoner',
	'pfunc_time_error'                      => 'Feil: ugyldig tid',
	'pfunc_time_too_long'                   => 'Feil: #time brukt for mange ganger',
	'pfunc_rel2abs_invalid_depth'           => 'Feil: Ugyldig dybde i sti: «$1» (prøvde å få tilgang til en node over rotnoden)',
	'pfunc_expr_stack_exhausted'            => 'Uttrykksfeil: Stakk utbrukt',
	'pfunc_expr_unexpected_number'          => 'Uttrykksfeil: Uventet nummer',
	'pfunc_expr_preg_match_failure'         => 'Uttrykksfeil: Uventet preg_match-feil',
	'pfunc_expr_unrecognised_word'          => 'Uttrykksfeil: Ugjenkjennelig ord «$1»',
	'pfunc_expr_unexpected_operator'        => 'Uttrykksfeil: Uventet $1-operator',
	'pfunc_expr_missing_operand'            => 'Uttrykksfeil: Mangler operand for $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Uttrykksfeil: Uventet lukkende parentes',
	'pfunc_expr_unrecognised_punctuation'   => 'Uttrykksfeil: Ugjenkjennelig tegn «$1»',
	'pfunc_expr_unclosed_bracket'           => 'Uttrykksfeil: Åpen parentes',
	'pfunc_expr_division_by_zero'           => 'Deling på null',
	'pfunc_expr_unknown_error'              => 'Uttrykksfeil: Ukjent feil ($1)',
	'pfunc_expr_not_a_number'               => 'I $1: resultat er ikke et tall',
	'pfunc_ifexist_warning'                 => 'Advarsel: Denne siden bruker #ifexist for mange ganger. Den burde brukes mindre enn $2 ganger, men brukes nå $1.',
	'pfunc_max_ifexist_category'            => 'Sider som bruker ifexist for mange ganger',
);

/** Occitan (Occitan)
 * @author Cedric31
 * @author Siebrand
 */
$messages['oc'] = array(
	'pfunc_desc'                            => 'Augmenta lo parsaire amb de foncions logicas',
	'pfunc_time_error'                      => 'Error: durada invalida',
	'pfunc_time_too_long'                   => 'Error: parser #time apelat tròp de còps',
	'pfunc_rel2abs_invalid_depth'           => 'Error: nivèl de repertòri invalid dins lo camin : "$1" (a ensajat d’accedir a un nivèl al-dessús del repertòri raiç)',
	'pfunc_expr_stack_exhausted'            => 'Expression erronèa : pila agotada',
	'pfunc_expr_unexpected_number'          => 'Expression erronèa : nombre pas esperat',
	'pfunc_expr_preg_match_failure'         => 'Expression erronèa : una expression pas compresa a pas capitat',
	'pfunc_expr_unrecognised_word'          => "Error d'expression : lo mot '''$1''' es pas reconegut",
	'pfunc_expr_unexpected_operator'        => "Error d'expression : l'operator '''$1''' es pas reconegut",
	'pfunc_expr_missing_operand'            => "Error d'expression : l'operanda '''$1''' es pas reconeguda",
	'pfunc_expr_unexpected_closing_bracket' => "Error d'expression : parentèsi tampanta pas prevista",
	'pfunc_expr_unrecognised_punctuation'   => "Error d'expression : caractèr de ponctuacion « $1 » pas reconegut",
	'pfunc_expr_unclosed_bracket'           => 'Error d’expression : parentèsi pas tampada',
	'pfunc_expr_division_by_zero'           => 'Division per zèro',
	'pfunc_expr_unknown_error'              => "Error d'expression : error desconeguda ($1)",
	'pfunc_expr_not_a_number'               => 'Dins $1 : lo resultat es pas un nombre',
	'pfunc_ifexist_warning'                 => "Atencion : Aquesta pagina conten tròp d'apèls a <code>#ifexist</code>. Ne deuriá aver mens que $2, alara que n'a $1.",
	'pfunc_max_ifexist_category'            => "Paginas amb tròp d'apèls a <code>#ifexist</code>",
);

/** Polish (Polski)
 * @author Derbeth
 * @author Sp5uhe
 * @author Siebrand
 */
$messages['pl'] = array(
	'pfunc_desc'                            => 'Rozszerza analizator składni o funkcje logiki',
	'pfunc_time_error'                      => 'Błąd: niepoprawny czas',
	'pfunc_time_too_long'                   => 'Błąd: za dużo wywołań funkcji #time',
	'pfunc_rel2abs_invalid_depth'           => 'Błąd: Nieprawidłowa głębokość w ścieżce: "$1" (próba dostępu do węzła powyżej korzenia)',
	'pfunc_expr_stack_exhausted'            => 'Błąd w wyrażeniu: Stos wyczerpany',
	'pfunc_expr_unexpected_number'          => 'Błąd w wyrażeniu: Niespodziewana liczba',
	'pfunc_expr_preg_match_failure'         => 'Błąd w wyrażeniu: Niespodziewany błąd w preg_match',
	'pfunc_expr_unrecognised_word'          => 'Błąd w wyrażeniu: Nierozpoznane słowo "$1"',
	'pfunc_expr_unexpected_operator'        => 'Błąd w wyrażeniu: Nieoczekiwany operator $1',
	'pfunc_expr_missing_operand'            => 'Błąd w wyrażeniu: Brakujący operand dla $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Błąd w wyrażeniu: Nieoczekiwany nawias zamykający',
	'pfunc_expr_unrecognised_punctuation'   => 'Błąd w wyrażeniu: Nierozpoznany znak interpunkcyjny "$1"',
	'pfunc_expr_unclosed_bracket'           => 'Błąd w wyrażeniu: Niedomknięty nawias',
	'pfunc_expr_division_by_zero'           => 'Dzielenie przez zero',
	'pfunc_expr_unknown_error'              => 'Błąd w wyrażeniu: Nieznany błąd ($1)',
	'pfunc_expr_not_a_number'               => 'W $1: wynik nie jest liczbą',
	'pfunc_ifexist_warning'                 => 'Uwaga: Ta strona zawiera zbyt wiele wywołań funkcji #ifexist. Nie ich może być więcej niż $2, a jest obecnie $1.',
	'pfunc_max_ifexist_category'            => 'Strony ze zbyt dużą ilością wywołań ifexist',
);

/** Piemontèis (Piemontèis)
 * @author Bèrto 'd Sèra
 * @author Siebrand
 */
$messages['pms'] = array(
	'pfunc_time_error'            => 'Eror: temp nen bon',
	'pfunc_time_too_long'         => 'Eror: #time a ven ciamà tròpe vire',
	'pfunc_rel2abs_invalid_depth' => 'Eror: profondità nen bon-a ant ël përcors: "$1" (a l\'é provasse a ciamé un grop dzora a la rèis)',
);

/** Pashto (پښتو)
 * @author Ahmed-Najib-Biabani-Ibrahimkhel
 */
$messages['ps'] = array(
	'pfunc_time_error' => 'ستونزه: ناسم وخت',
);

/** Portuguese (Português)
 * @author Malafaya
 */
$messages['pt'] = array(
	'pfunc_desc'                            => 'Melhora o analisador "parser" com funções lógicas',
	'pfunc_time_error'                      => 'Erro: tempo inválido',
	'pfunc_time_too_long'                   => 'Erro: demasiadas chamadas a #time',
	'pfunc_rel2abs_invalid_depth'           => 'Erro: Profundidade inválida no caminho: "$1" (foi tentado o acesso a um nó acima do nó raiz)',
	'pfunc_expr_stack_exhausted'            => 'Erro de expressão: Pilha esgotada',
	'pfunc_expr_unexpected_number'          => 'Erro de expressão: Número inesperado',
	'pfunc_expr_preg_match_failure'         => 'Erro de expressão: Falha em preg_match inesperada',
	'pfunc_expr_unrecognised_word'          => 'Erro de expressão: Palavra "$1" não reconhecida',
	'pfunc_expr_unexpected_operator'        => 'Erro de expressão: Operador $1 inesperado',
	'pfunc_expr_missing_operand'            => 'Erro de expressão: Falta operando para $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Erro de expressão: Parêntese de fecho inesperado',
	'pfunc_expr_unrecognised_punctuation'   => 'Erro de expressão: Caracter de pontuação "$1" não reconhecido',
	'pfunc_expr_unclosed_bracket'           => 'Erro de expressão: Parêntese não fechado',
	'pfunc_expr_division_by_zero'           => 'Divisão por zero',
	'pfunc_expr_unknown_error'              => 'Erro de expressão: Erro desconhecido ($1)',
	'pfunc_expr_not_a_number'               => 'Em $1: resultado não é um número',
	'pfunc_ifexist_warning'                 => 'Aviso: Esta página contém demasiadas chamadas #ifexist. Deverá ter menos de $2, mas neste momento existem $1.',
	'pfunc_max_ifexist_category'            => "Páginas com demasiadas chamadas 'ifexist'",
);

/** Russian (Русский)
 * @author .:Ajvol:.
 */
$messages['ru'] = array(
	'pfunc_desc'                            => 'Улучшенный синтаксический анализатор с логическими функциями',
	'pfunc_time_error'                      => 'Ошибка: неправильное время',
	'pfunc_time_too_long'                   => 'Ошибка: слишком много вызовов функции #time',
	'pfunc_rel2abs_invalid_depth'           => 'Ошибка: ошибочная глубина пути: «$1» (попытка доступа к узлу, находящемуся выше, чем корневой)',
	'pfunc_expr_stack_exhausted'            => 'Ошибка выражения: переполнение стека',
	'pfunc_expr_unexpected_number'          => 'Ошибка выражения: неожидаемое число',
	'pfunc_expr_preg_match_failure'         => 'Ошибка выражения: сбой preg_match',
	'pfunc_expr_unrecognised_word'          => 'Ошибка выражения: неопознанное слово «$1»',
	'pfunc_expr_unexpected_operator'        => 'Ошибка выражения: неожидаемый оператор $1',
	'pfunc_expr_missing_operand'            => 'Ошибка выражения: $1 не хватает операнда',
	'pfunc_expr_unexpected_closing_bracket' => 'Ошибка выражения: неожидаемая закрывающая скобка',
	'pfunc_expr_unrecognised_punctuation'   => 'Ошибка выражения: неопознанный символ пунктуации «$1»',
	'pfunc_expr_unclosed_bracket'           => 'Ошибка выражения: незакрытая скобка',
	'pfunc_expr_division_by_zero'           => 'Деление на ноль',
	'pfunc_expr_unknown_error'              => 'Ошибка выражения: неизвестная ошибка ($1)',
	'pfunc_expr_not_a_number'               => 'В $1: результат не является числом',
	'pfunc_ifexist_warning'                 => 'Внимание: эта страница содержит $1 {{PLURAL:$1|вызов|вызова|вызовов}} #ifexist при ограничении в $2.',
	'pfunc_max_ifexist_category'            => 'Страницы, в которых слишком много вызовов ifexist',
);

/** Yakut (Саха тыла)
 * @author HalanTul
 */
$messages['sah'] = array(
	'pfunc_desc'                            => 'Логическай функциялаах тупсарыллыбыт синтаксическай анализатор',
	'pfunc_time_error'                      => 'Алҕас: сыыһа кэм',
	'pfunc_time_too_long'                   => 'Алҕас: #time функция наһаа элбэхтик хатыламмыт',
	'pfunc_rel2abs_invalid_depth'           => 'Алҕас: ошибочная глубина пути: «$1» (попытка доступа к узлу, находящемуся выше, чем корневой)',
	'pfunc_expr_stack_exhausted'            => 'Ошибка выражения: переполнение стека',
	'pfunc_expr_unexpected_number'          => 'Алҕас: кэтэһиллибэтэх чыыһыла',
	'pfunc_expr_preg_match_failure'         => 'Алҕас: preg_match моһуоктанна',
	'pfunc_expr_unrecognised_word'          => 'Алҕас: биллибэт тыл «$1»',
	'pfunc_expr_unexpected_operator'        => 'Алҕас: кэтэһиллибэтэх оператор $1',
	'pfunc_expr_missing_operand'            => 'Алҕас: $1 операнда тиийбэт',
	'pfunc_expr_unexpected_closing_bracket' => 'Алҕас: кэтэһиллибэтэх сабар ускуопка',
	'pfunc_expr_unrecognised_punctuation'   => 'Алҕас: биллибэт пунктуация бэлиэтэ «$1»',
	'pfunc_expr_unclosed_bracket'           => 'Алҕас: сабыллыбатах ускуопка',
	'pfunc_expr_division_by_zero'           => 'Нуулга түҥэттии',
	'pfunc_expr_unknown_error'              => 'Expression error (ошибка выражения): Биллибэт алҕас ($1)',
	'pfunc_expr_not_a_number'               => '$1 иһигэр: эппиэтэ чыыһыла буолбатах',
	'pfunc_ifexist_warning'                 => 'Болҕой: бу сирэй $1 {{PLURAL:$1|ыҥырыылаах|ыҥырыылардаах}}, #ifexist баччаннан хааччахтаммытын үрдүнэн $2.',
	'pfunc_max_ifexist_category'            => 'ifexist ыҥырыылар наһаа элбэхтик көстөр сирэйдэрэ',
);

/** Slovak (Slovenčina)
 * @author Helix84
 */
$messages['sk'] = array(
	'pfunc_desc'                            => 'Rozšírenie syntaktického analyzátora o logické funkcie',
	'pfunc_time_error'                      => 'Chyba: Neplatný čas',
	'pfunc_time_too_long'                   => 'Chyba: príliš veľa volaní #time',
	'pfunc_rel2abs_invalid_depth'           => 'Chyba: Neplatná hĺbka v ceste: „$1“ (pokus o prístup k uzlu nad koreňovým uzlom)',
	'pfunc_expr_stack_exhausted'            => 'Chyba výrazu: Zásobník vyčerpaný',
	'pfunc_expr_unexpected_number'          => 'Chyba výrazu: Neočakávané číslo',
	'pfunc_expr_preg_match_failure'         => 'Chyba výrazu: Neočakávané zlyhanie funkcie preg_match',
	'pfunc_expr_unrecognised_word'          => 'Chyba výrazu: Nerozpoznané slovo „$1“',
	'pfunc_expr_unexpected_operator'        => 'Chyba výrazu: Neočakávaný operátor $1',
	'pfunc_expr_missing_operand'            => 'Chyba výrazu: Chýbajúci operand pre $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Chyba výrazu: Neočakávaná zatvárajúca hranatá zátvorka',
	'pfunc_expr_unrecognised_punctuation'   => 'Chyba výrazu: Nerozpoznané diakritické znamienko „$1“',
	'pfunc_expr_unclosed_bracket'           => 'Chyba výrazu: Neuzavretá hranatá zátvorka',
	'pfunc_expr_division_by_zero'           => 'Chyba výrazu: Delenie nulou',
	'pfunc_expr_unknown_error'              => 'Chyba výrazu: Neznáma chyba ($1)',
	'pfunc_expr_not_a_number'               => 'V $1: výsledok nie je číslo',
	'pfunc_ifexist_warning'                 => 'Upozornenie: Táto stránka obsahuje príliš veľa volaní #ifexist. Malo by ich byť menej ako $2, momentálne ich je $1.',
	'pfunc_max_ifexist_category'            => 'Stránky s príliš veľkým počtom volaní ifexist',
);

/** Seeltersk (Seeltersk)
 * @author Pyt
 */
$messages['stq'] = array(
	'pfunc_time_error'                      => 'Failer: uungultige Tiedangoawe',
	'pfunc_time_too_long'                   => 'Failer: tou fuul #time-Aproupe',
	'pfunc_rel2abs_invalid_depth'           => 'Failer: uungultige Djüpte in Paad: „$1“ (Fersäik, ap n Knättepunkt buppe dän Haudknättepunkt toutougriepen)',
	'pfunc_expr_stack_exhausted'            => 'Expression-Failer: Stack-Uurloop',
	'pfunc_expr_unexpected_number'          => 'Expression-Failer: Nit ferwachtede Taal',
	'pfunc_expr_preg_match_failure'         => 'Expression-Failer: Uunferwachtede „preg_match“-Failfunktion',
	'pfunc_expr_unrecognised_word'          => 'Expression-Failer: Nit wierkoand Woud „$1“',
	'pfunc_expr_unexpected_operator'        => 'Expression-Failer: Uunferwachteden Operator: <strong><tt>$1</tt></strong>',
	'pfunc_expr_missing_operand'            => 'Expression-Failer: Failenden Operand foar <strong><tt>$1</tt></strong>',
	'pfunc_expr_unexpected_closing_bracket' => 'Expression-Failer: Uunferwachte sluutende kaantige Klammere',
	'pfunc_expr_unrecognised_punctuation'   => 'Expression-Failer: Nit wierkoand Satsteeken „$1“',
	'pfunc_expr_unclosed_bracket'           => 'Expression-Failer: Nit sleetene kaantige Klammer',
	'pfunc_expr_division_by_zero'           => 'Expression-Failer: Division truch Null',
	'pfunc_expr_unknown_error'              => 'Expression-Failer: Uunbekoanden Failer ($1)',
	'pfunc_expr_not_a_number'               => 'Expression-Failer: In $1: Resultoat is neen Taal',
	'pfunc_ifexist_warning'                 => 'Woarschauenge: Disse Siede änthaalt toufuul #ifexist-Aproupe. Der duuren maximoal $2 Aproupe weese, der sunt oawers $1 Aproupe.',
	'pfunc_max_ifexist_category'            => 'Sieden mäd toufuul ifexist-Aproupe',
);

/** Swedish (Svenska)
 * @author Lejonel
 */
$messages['sv'] = array(
	'pfunc_desc'                            => 'Lägger till logiska funktioner i parsern',
	'pfunc_time_error'                      => 'Fel: ogiltig tid',
	'pfunc_time_too_long'                   => 'Fel: för många anrop av #time',
	'pfunc_rel2abs_invalid_depth'           => 'Fel: felaktig djup i sökväg: "$1" (försöker nå en nod ovanför rotnoden)',
	'pfunc_expr_stack_exhausted'            => 'Fel i uttryck: Stackutrymmet tog slut',
	'pfunc_expr_unexpected_number'          => 'Fel i uttryck: Oväntat tal',
	'pfunc_expr_preg_match_failure'         => 'Fel i uttryck: Oväntad fel i preg_match',
	'pfunc_expr_unrecognised_word'          => 'Fel i uttryck: Okänt ord "$1"',
	'pfunc_expr_unexpected_operator'        => 'Fel i uttryck: Oväntad operator $1',
	'pfunc_expr_missing_operand'            => 'Fel i uttryck: Operand saknas för $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Fel i uttryck: Oväntad avslutande parentes',
	'pfunc_expr_unrecognised_punctuation'   => 'Fel i uttryck: Okänt interpunktionstecken "$1"',
	'pfunc_expr_unclosed_bracket'           => 'Fel i uttryck: Oavslutad parentes',
	'pfunc_expr_division_by_zero'           => 'Division med noll',
	'pfunc_expr_unknown_error'              => 'Fel i uttryck: Okänt fel ($1)',
	'pfunc_expr_not_a_number'               => 'I $1: resultatet är inte ett tal',
	'pfunc_ifexist_warning'                 => 'Varning: Den här sidan innehåller för många anrop av #ifexist. Antalet anrop måste vara mindre än $2, nu är det $1.',
	'pfunc_max_ifexist_category'            => 'Sidor med för många ifexist-anrop',
);

/** Telugu (తెలుగు)
 * @author Mpradeep
 * @author Veeven
 */
$messages['te'] = array(
	'pfunc_time_error'                      => 'లోపం: సమయం సరిగ్గా లేదు',
	'pfunc_time_too_long'                   => 'లోపం: #timeను చాలా సార్లు ఉపయోగించారు',
	'pfunc_rel2abs_invalid_depth'           => 'లోపం: పాత్ యొక్క డెప్తు సరిగ్గాలేదు: "$1" (రూట్ నోడు కంటే పైన ఉన్న నోడు ఉపయోగించటానికి ప్రయత్నం జరిగింది)',
	'pfunc_expr_stack_exhausted'            => 'సమాసంలో(Expression) లోపం: స్టాకు మొత్తం అయిపోయింది',
	'pfunc_expr_unexpected_number'          => 'సమాసంలో(Expression) లోపం: ఊహించని సంఖ్య వచ్చింది',
	'pfunc_expr_preg_match_failure'         => 'సమాసంలో(Expression) లోపం: preg_matchలో ఊహించని విఫలం',
	'pfunc_expr_unrecognised_word'          => 'సమాసంలో(Expression) లోపం: "$1" అనే పదాన్ని గుర్తుపట్టలేకపోతున్నాను',
	'pfunc_expr_unexpected_operator'        => 'సమాసంలో(Expression) లోపం: $1 పరికర్తను(operator) ఊహించలేదు',
	'pfunc_expr_missing_operand'            => 'సమాసంలో(Expression) లోపం: $1కు ఒక ఆపరాండును ఇవ్వలేదు',
	'pfunc_expr_unexpected_closing_bracket' => 'సమాసంలో(Expression) లోపం: ఊహించని బ్రాకెట్టు ముగింపు',
	'pfunc_expr_unrecognised_punctuation'   => 'సమాసంలో(Expression) లోపం: "$1" అనే విరామ చిహ్నాన్ని గుర్తించలేకపోతున్నాను',
	'pfunc_expr_unclosed_bracket'           => 'సమాసంలో(Expression) లోపం: బ్రాకెట్టును మూయలేదు',
	'pfunc_expr_division_by_zero'           => 'సున్నాతో భాగించారు',
	'pfunc_expr_unknown_error'              => 'సమాసంలో(Expression) లోపం: తెలియని లోపం ($1)',
	'pfunc_expr_not_a_number'               => '$1లో: వచ్చిన ఫలితం సంఖ్య కాదు',
	'pfunc_ifexist_warning'                 => 'హెచ్చరిక: ఈ పేజీలో #ifexistలను చాలాసార్లు ఉపయోగించారు. ప్రస్తుతం $1 సార్లు ఉపయోగించారు, కానీ $2 సార్లు కంటే ఎక్కువ సార్లు ఉపయోగించకూడదు.',
	'pfunc_max_ifexist_category'            => 'ifexistలను మరీ ఎక్కువగా ఉపయోగిస్తున్న పేజీలు.',
);

/** Tajik (Тоҷикӣ)
 * @author Ibrahim
 */
$messages['tg'] = array(
	'pfunc_expr_division_by_zero' => 'Тақсим бар сифр',
);

/** Vietnamese (Tiếng Việt)
 * @author Vinhtantran
 * @author Minh Nguyen
 */
$messages['vi'] = array(
	'pfunc_desc'                            => 'Nâng cao bộ xử lý với những hàm cú pháp lôgic',
	'pfunc_time_error'                      => 'Lỗi: thời gian không hợp lệ',
	'pfunc_time_too_long'                   => 'Lỗi: quá nhiều lần gọi #time',
	'pfunc_rel2abs_invalid_depth'           => 'Lỗi: độ sâu không hợp lệ trong đường dẫn “$1” (do cố gắng truy cập nút phía trên nút gốc)',
	'pfunc_expr_stack_exhausted'            => 'Lỗi biểu thức: Đã cạn stack',
	'pfunc_expr_unexpected_number'          => 'Lỗi biểu thức: Dư số',
	'pfunc_expr_preg_match_failure'         => 'Lỗi biểu thức: Hàm preg_match thất bại',
	'pfunc_expr_unrecognised_word'          => 'Lỗi biểu thức: Từ “$1” không rõ ràng',
	'pfunc_expr_unexpected_operator'        => "Lỗi biểu thức: Dư toán tử '''$1'''",
	'pfunc_expr_missing_operand'            => 'Lỗi biểu thức: Thiếu toán hạng trong $1',
	'pfunc_expr_unexpected_closing_bracket' => 'Lỗi biểu thức: Dư dấu đóng ngoặc',
	'pfunc_expr_unrecognised_punctuation'   => 'Lỗi biểu thức: Dấu câu “$1” không rõ ràng',
	'pfunc_expr_unclosed_bracket'           => 'Lỗi biểu thức: Dấu ngoặc chưa được đóng',
	'pfunc_expr_division_by_zero'           => 'Chia cho zero',
	'pfunc_expr_unknown_error'              => 'Lỗi biểu thức: Lỗi không rõ nguyên nhân ($1)',
	'pfunc_expr_not_a_number'               => 'Trong $1: kết quả không phải là kiểu số',
	'pfunc_ifexist_warning'                 => 'Cảnh báo: Trang này có quá nhiều lần gọi hàm #ifexist. Số lần gọi nên ít hơn $2, hiện có đến $1 lần gọi.',
	'pfunc_max_ifexist_category'            => 'Trang có quá nhiều hàm gọi ifexist',
);

/** Volapük (Volapük)
 * @author Smeira
 */
$messages['vo'] = array(
	'pfunc_time_error'            => 'Pök: tim no lonöföl',
	'pfunc_expr_division_by_zero' => 'Müedam dub ser',
	'pfunc_expr_not_a_number'     => 'In $1: sek no binon num',
);

/** 粵語 (粵語) */
$messages['yue'] = array(
	'pfunc_time_error'            => '錯: 唔啱嘅時間',
	'pfunc_time_too_long'         => '錯: 太多 #time 呼叫',
	'pfunc_rel2abs_invalid_depth' => '錯: 唔啱路徑嘅深度: "$1" (已經試過由頭點落個點度)',
);

/** Simplified Chinese (‪中文(简体)‬) */
$messages['zh-hans'] = array(
	'pfunc_time_error'            => '错误: 不正确的时间',
	'pfunc_time_too_long'         => '错误: 过多 #time 的呼叫',
	'pfunc_rel2abs_invalid_depth' => '错误: 不正确的路径深度: "$1" (已经尝试在顶点访问该点)',
);

/** Traditional Chinese (‪中文(繁體)‬) */
$messages['zh-hant'] = array(
	'pfunc_time_error'            => '錯誤: 不正確的時間',
	'pfunc_time_too_long'         => '錯誤: 過多 #time 的呼叫',
	'pfunc_rel2abs_invalid_depth' => '錯誤: 不正確的路徑深度: "$1" (已經嘗試在頂點存取該點)',
);

