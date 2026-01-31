// main.cpp
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTextStream>
#include <QStringList>
#include <QDate>
#include <optional>

#include "core/repos/counterparty_repo.h"
#include "core/services/counterparty_service.h"

// (добавяш други includes, когато имплементираш другите targets)
#include "core/repos/invoice_repo.h"
#include "core/repos/invoice_line_repo.h"
#include "core/services/invoice_service.h"
#include "core/repos/invoice_line_repo.h"
#include "core/services/invoice_line_service.h"

// Example commands (one per action/target):
// Counterparty:
//   dopek create counterparty --name "ACME LTD" --eik 123456789 --vat 1 --mol "Ivan Petrov" --address "Sofia Center" --email "a@b.com"
//   dopek edit counterparty --id 1 --name "ACME Updated" --vat 0
//   dopek get counterparty --id 1
//   dopek list counterparty
//   dopek delete counterparty --id 1
// Invoice:
//   dopek create invoice --number INV-001 --cp 1 --date 2024-01-10 --dir sale
//   dopek edit invoice --id 1 --number INV-002
//   dopek get invoice --id 1
//   dopek list invoice
//   dopek delete invoice --id 1
// Invoice line:
//   dopek create invoice-line --invoice 1 --desc "Consulting hours" --qty 2 --price 150 --unit hour --type services --line-no 1
//   dopek get invoice-line --id 1
//   dopek list invoice-line --invoice 1
//   dopek delete invoice-line --id 1

static QTextStream out(stdout);
static QTextStream err(stderr);

static int fail(const QString& msg) {
    err << "ERROR: " << msg << "\n";
    return 1;
}

static void print_kv(const QString& key, const QString& value) {
    out << key << "=" << value << "\n";
}

static QString formatOrDefault(const QCommandLineParser& p) {
    const QString f = p.value("format").trimmed();
    return f.isEmpty() ? "human" : f;
}

static void addCommonOptions(QCommandLineParser& p) {
    p.addHelpOption();
    p.addVersionOption();
    p.addOption(QCommandLineOption(QStringList() << "f" << "format",
                                   "Output format: human|kv (default human).",
                                   "format",
                                   "human"));
}

static bool isHuman(const QString& fmt) { return fmt == "human"; }
static bool isKv(const QString& fmt) { return fmt == "kv"; }

static std::optional<QString> parseFormat(const QCommandLineParser& p) {
    const QString fmt = formatOrDefault(p);
    if (!isHuman(fmt) && !isKv(fmt)) return std::nullopt;
    return fmt;
}

static QString quoteIfNeeded(const QString& value) {
    if (value.contains(' ')) return QString("\"%1\"").arg(value);
    return value;
}

static std::optional<qint64> parseInt64(const QString& value) {
    bool ok = false;
    const qint64 parsed = value.toLongLong(&ok);
    if (!ok) return std::nullopt;
    return parsed;
}

static std::optional<int> parseInt(const QString& value) {
    bool ok = false;
    const int parsed = value.toInt(&ok);
    if (!ok) return std::nullopt;
    return parsed;
}

static std::optional<double> parseDouble(const QString& value) {
    bool ok = false;
    const double parsed = value.toDouble(&ok);
    if (!ok) return std::nullopt;
    return parsed;
}

struct CommandHead {
    QString action;  // create|edit|get|list|delete
    QString target;  // counterparty|invoice|...
    QStringList tailArgs; // options after action+target
};

static bool parseHead(const QStringList& argv, CommandHead& h) {
    // argv[0] is program name
    if (argv.size() < 3) return false;
    h.action = argv[1].trimmed();
    h.target = argv[2].trimmed();
    h.tailArgs = argv.mid(3);
    return true;
}

static int showTopHelp(const QString& prog) {
    out << "Usage:\n"
        << "  " << prog << " <action> <target> [--format human|kv] [options]\n\n"
        << "Actions:\n"
        << "  create | edit | get | list | delete\n\n"
        << "Targets:\n"
        << "  counterparty (cp) | invoice (inv) | invoice-line (line)\n\n"
        << "Examples:\n"
        << "  " << prog << " create counterparty --name \"ACME\" --eik 123456789 --vat 1 --mol \"Ivan\" --address \"Sofia\" --email \"a@b.com\"\n"
        << "  " << prog << " --format kv list counterparty\n";
    return 1;
}

static std::optional<QDate> parseDateYmd(const QString& s) {
    const auto d = QDate::fromString(s.trimmed(), "yyyy-MM-dd");
    if (!d.isValid()) return std::nullopt;
    return d;
}

static void printInvoiceHuman(const Invoice& inv) {
    out << "Invoice #" << inv.id << "\n"
        << "  number:   " << inv.number << "\n"
        << "  cp_id:    " << inv.counterparty_id << "\n"
        << "  date:     " << inv.date.toString("yyyy-MM-dd") << "\n"
        << "  dir:      " << inv.direction << "\n"
        << "  net:      " << inv.net_amount << "\n";
}

static void printInvoiceKv(const Invoice& inv) {
    out << "id=" << inv.id
        << " number=" << quoteIfNeeded(inv.number)
        << " cp_id=" << inv.counterparty_id
        << " date=" << inv.date.toString("yyyy-MM-dd")
        << " dir=" << inv.direction
        << " net_amount=" << inv.net_amount
        << "\n";
}

static void printInvoiceLineHuman(const InvoiceLine& l) {
    out << "InvoiceLine #" << l.id << "\n"
        << "  invoice_id: " << l.invoice_id << "\n"
        << "  line_no:    " << l.line_no << "\n"
        << "  desc:       " << l.description << "\n"
        << "  type:       " << l.line_type << "\n"
        << "  qty:        " << l.qty << "\n"
        << "  unit:       " << l.unit << "\n"
        << "  unit_price: " << l.unit_price << "\n"
        << "  net:        " << l.net_amount << "\n";
}

static void printInvoiceLineKv(const InvoiceLine& l) {
    out << "id=" << l.id
        << " invoice_id=" << l.invoice_id
        << " line_no=" << l.line_no
        << " desc=" << quoteIfNeeded(l.description)
        << " type=" << l.line_type
        << " qty=" << l.qty
        << " unit=" << quoteIfNeeded(l.unit)
        << " price=" << l.unit_price
        << " net_amount=" << l.net_amount
        << "\n";
}


// -------------------------
// Counterparty handlers
// -------------------------

static void printCounterpartyHuman(const Counterparty& c) {
    out << "Counterparty #" << c.id << "\n"
        << "  name:    " << c.name << "\n"
        << "  eik:     " << c.eik << "\n"
        << "  vat_reg: " << (c.vat_reg ? "true" : "false") << "\n"
        << "  mol:     " << c.mol << "\n"
        << "  address: " << c.address << "\n"
        << "  email:   " << c.email << "\n";
}

static void printCounterpartyKv(const Counterparty& c) {
    out << "id=" << c.id
        << " name=" << quoteIfNeeded(c.name)
        << " eik=" << c.eik
        << " vat_reg=" << (c.vat_reg ? "1" : "0")
        << " mol=" << quoteIfNeeded(c.mol)
        << " address=" << quoteIfNeeded(c.address)
        << " email=" << quoteIfNeeded(c.email)
        << "\n";
}

static int handleCounterpartyCreate(const QStringList& args, CounterpartyService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek create counterparty");
    addCommonOptions(p);

    QCommandLineOption nameOpt("name", "Name.", "name");
    QCommandLineOption eikOpt("eik", "EIK (9 digits).", "eik");
    QCommandLineOption vatOpt("vat", "VAT registered: 0|1.", "vat");
    QCommandLineOption molOpt("mol", "MOL.", "mol");
    QCommandLineOption addressOpt("address", "Address.", "address");
    QCommandLineOption emailOpt("email", "Email.", "email");

    p.addOption(nameOpt);
    p.addOption(eikOpt);
    p.addOption(vatOpt);
    p.addOption(molOpt);
    p.addOption(addressOpt);
    p.addOption(emailOpt);

    if (!p.parse(args)) return fail(p.errorText());

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    if (!p.isSet(nameOpt)) return fail("--name is required");
    if (!p.isSet(eikOpt)) return fail("--eik is required");
    if (!p.isSet(vatOpt)) return fail("--vat is required");
    if (!p.isSet(molOpt)) return fail("--mol is required");
    if (!p.isSet(addressOpt)) return fail("--address is required");
    const QString name = p.value(nameOpt);
    const QString eik = p.value(eikOpt);
    const QString mol = p.value(molOpt);
    const QString address = p.value(addressOpt);
    const QString email = p.value(emailOpt);

    const QString vatRaw = p.value(vatOpt).trimmed();
    if (vatRaw != "0" && vatRaw != "1") return fail("--vat must be 0|1");
    const bool vat_reg = (vatRaw == "1");

    auto r = svc.add(name, eik, vat_reg, mol, address, email);
    if (!r.ok) return fail(r.error);

    if (isKv(fmt)) print_kv("id", QString::number(r.value));
    else out << "Created counterparty id=" << r.value << "\n";

    return 0;
}

static int handleCounterpartyGet(const QStringList& args, CounterpartyService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek get counterparty");
    addCommonOptions(p);

    QCommandLineOption idOpt("id", "Id.", "id");
    p.addOption(idOpt);

    if (!p.parse(args)) return fail(p.errorText());
    if (!p.isSet(idOpt)) return fail("--id is required");

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    const auto idParsed = parseInt64(p.value(idOpt));
    if (!idParsed.has_value()) return fail("--id must be integer");
    const qint64 id = idParsed.value();

    auto r = svc.get(id);
    if (!r.ok) return fail(r.error);

    if (isKv(fmt)) printCounterpartyKv(r.value);
    else printCounterpartyHuman(r.value);

    return 0;
}

static int handleCounterpartyList(const QStringList& args, CounterpartyService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek list counterparty");
    addCommonOptions(p);

    if (!p.parse(args)) return fail(p.errorText());
    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();

    auto r = svc.list();
    if (!r.ok) return fail(r.error);

    const auto& list = r.value; // avoid detach warnings
    for (const Counterparty& c : list) {
        if (isKv(fmt)) {
            printCounterpartyKv(c);
        } else {
            printCounterpartyHuman(c);
            out << "\n";
        }
    }
    return 0;
}

static int handleCounterpartyEdit(const QStringList& args, CounterpartyService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek edit counterparty");
    addCommonOptions(p);

    QCommandLineOption idOpt("id", "Id.", "id");
    QCommandLineOption nameOpt("name", "Name.", "name");
    QCommandLineOption eikOpt("eik", "EIK (9 digits).", "eik");
    QCommandLineOption vatOpt("vat", "VAT registered: 0|1.", "vat");
    QCommandLineOption molOpt("mol", "MOL.", "mol");
    QCommandLineOption addressOpt("address", "Address.", "address");
    QCommandLineOption emailOpt("email", "Email.", "email");

    p.addOption(idOpt);
    p.addOption(nameOpt);
    p.addOption(eikOpt);
    p.addOption(vatOpt);
    p.addOption(molOpt);
    p.addOption(addressOpt);
    p.addOption(emailOpt);

    if (!p.parse(args)) return fail(p.errorText());
    if (!p.isSet(idOpt)) return fail("--id is required");

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    const auto idParsed = parseInt64(p.value(idOpt));
    if (!idParsed.has_value()) return fail("--id must be integer");
    const qint64 id = idParsed.value();

    std::optional<QString> name, eik, mol, address, email;
    std::optional<bool> vat_reg;

    if (p.isSet(nameOpt)) name = p.value(nameOpt);
    if (p.isSet(eikOpt)) eik = p.value(eikOpt);
    if (p.isSet(molOpt)) mol = p.value(molOpt);
    if (p.isSet(addressOpt)) address = p.value(addressOpt);
    if (p.isSet(emailOpt)) email = p.value(emailOpt);
    if (p.isSet(vatOpt)) {
        const QString vatRaw = p.value(vatOpt).trimmed();
        if (vatRaw != "0" && vatRaw != "1") return fail("--vat must be 0|1");
        vat_reg = (vatRaw == "1");
    }

    auto r = svc.edit(id, name, eik, vat_reg, mol, address, email);
    if (!r.ok) return fail(r.error);

    if (isKv(fmt)) print_kv("ok", "1");
    else out << "Edited counterparty id=" << id << "\n";

    return 0;
}

static int handleCounterpartyDelete(const QStringList& args, CounterpartyRepo& repo) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek delete counterparty");
    addCommonOptions(p);

    QCommandLineOption idOpt("id", "Id.", "id");
    p.addOption(idOpt);

    if (!p.parse(args)) return fail(p.errorText());
    if (!p.isSet(idOpt)) return fail("--id is required");

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    const auto idParsed = parseInt64(p.value(idOpt));
    if (!idParsed.has_value()) return fail("--id must be integer");
    const qint64 id = idParsed.value();

    if (!repo.remove(id)) return fail("counterparty not found");

    if (isKv(fmt)) print_kv("ok", "1");
    else out << "Deleted counterparty id=" << id << "\n";

    return 0;
}

static int dispatchCounterparty(const QString& action, const QStringList& cmdArgs,
                                CounterpartyService& svc, CounterpartyRepo& repo) {
    // cmdArgs is like: { "dopek", ...options... }
    if (action == "create") return handleCounterpartyCreate(cmdArgs, svc);
    if (action == "get")    return handleCounterpartyGet(cmdArgs, svc);
    if (action == "list")   return handleCounterpartyList(cmdArgs, svc);
    if (action == "edit")   return handleCounterpartyEdit(cmdArgs, svc);
    if (action == "delete") return handleCounterpartyDelete(cmdArgs, repo);
    return fail("unsupported action for counterparty");
}

static int handleInvoiceCreate(const QStringList& args, InvoiceService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek create invoice");
    addCommonOptions(p);

    QCommandLineOption numberOpt("number", "Invoice number.", "number");
    QCommandLineOption cpOpt("cp", "Counterparty id.", "cp");
    QCommandLineOption dateOpt("date", "Date YYYY-MM-DD.", "date");
    QCommandLineOption dirOpt("dir", "Direction: purchase|sale.", "dir");

    p.addOption(numberOpt);
    p.addOption(cpOpt);
    p.addOption(dateOpt);
    p.addOption(dirOpt);

    if (!p.parse(args)) return fail(p.errorText());

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();

    if (!p.isSet(numberOpt)) return fail("--number is required");
    if (!p.isSet(cpOpt)) return fail("--cp is required");
    if (!p.isSet(dateOpt)) return fail("--date is required");
    if (!p.isSet(dirOpt)) return fail("--dir is required");

    const QString number = p.value(numberOpt);
    const auto cpParsed = parseInt64(p.value(cpOpt));
    if (!cpParsed.has_value()) return fail("--cp must be integer");
    const qint64 cp_id = cpParsed.value();

    auto d = parseDateYmd(p.value(dateOpt));
    if (!d.has_value()) return fail("--date invalid (YYYY-MM-DD)");

    const QString dir = p.value(dirOpt).trimmed();
    if (dir != "purchase" && dir != "sale") return fail("--dir must be purchase|sale");

    auto r = svc.add(number, cp_id, d.value(), dir);
    if (!r.ok) return fail(r.error);

    if (isKv(fmt)) print_kv("id", QString::number(r.value));
    else out << "Created invoice id=" << r.value << "\n";

    return 0;
}

static int handleInvoiceGet(const QStringList& args, InvoiceService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek get invoice");
    addCommonOptions(p);

    QCommandLineOption idOpt("id", "Id.", "id");
    p.addOption(idOpt);

    if (!p.parse(args)) return fail(p.errorText());
    if (!p.isSet(idOpt)) return fail("--id is required");

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    const auto idParsed = parseInt64(p.value(idOpt));
    if (!idParsed.has_value()) return fail("--id must be integer");
    const qint64 id = idParsed.value();

    auto r = svc.get(id);
    if (!r.ok) return fail(r.error);

    if (isKv(fmt)) printInvoiceKv(r.value);
    else printInvoiceHuman(r.value);

    return 0;
}

static int handleInvoiceList(const QStringList& args, InvoiceService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek list invoice");
    addCommonOptions(p);

    if (!p.parse(args)) return fail(p.errorText());
    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();

    auto r = svc.list();
    if (!r.ok) return fail(r.error);

    const auto& list = r.value; // avoid detach
    for (const Invoice& inv : list) {
        if (isKv(fmt)) {
            printInvoiceKv(inv);
        } else {
            printInvoiceHuman(inv);
            out << "\n";
        }
    }
    return 0;
}

static int handleInvoiceEdit(const QStringList& args, InvoiceService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek edit invoice");
    addCommonOptions(p);

    QCommandLineOption idOpt("id", "Id.", "id");
    QCommandLineOption numberOpt("number", "Invoice number.", "number");
    QCommandLineOption cpOpt("cp", "Counterparty id.", "cp");
    QCommandLineOption dateOpt("date", "Date YYYY-MM-DD.", "date");
    QCommandLineOption dirOpt("dir", "Direction: purchase|sale.", "dir");

    p.addOption(idOpt);
    p.addOption(numberOpt);
    p.addOption(cpOpt);
    p.addOption(dateOpt);
    p.addOption(dirOpt);

    if (!p.parse(args)) return fail(p.errorText());
    if (!p.isSet(idOpt)) return fail("--id is required");

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    const auto idParsed = parseInt64(p.value(idOpt));
    if (!idParsed.has_value()) return fail("--id must be integer");
    const qint64 id = idParsed.value();

    std::optional<QString> number;
    std::optional<qint64> cp_id;
    std::optional<QDate> date;
    std::optional<QString> dir;

    if (p.isSet(numberOpt)) number = p.value(numberOpt);

    if (p.isSet(cpOpt)) {
        const auto parsed = parseInt64(p.value(cpOpt));
        if (!parsed.has_value()) return fail("--cp must be integer");
        cp_id = parsed.value();
    }

    if (p.isSet(dateOpt)) {
        auto d = parseDateYmd(p.value(dateOpt));
        if (!d.has_value()) return fail("--date invalid (YYYY-MM-DD)");
        date = d.value();
    }

    if (p.isSet(dirOpt)) {
        const QString v = p.value(dirOpt).trimmed();
        if (v != "purchase" && v != "sale") return fail("--dir must be purchase|sale");
        dir = v;
    }

    auto r = svc.edit(id, number, cp_id, date, dir);
    if (!r.ok) return fail(r.error);

    if (isKv(fmt)) print_kv("ok", "1");
    else out << "Edited invoice id=" << id << "\n";

    return 0;
}

static int handleInvoiceDelete(const QStringList& args, InvoiceRepo& repo) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek delete invoice");
    addCommonOptions(p);

    QCommandLineOption idOpt("id", "Id.", "id");
    p.addOption(idOpt);

    if (!p.parse(args)) return fail(p.errorText());
    if (!p.isSet(idOpt)) return fail("--id is required");

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    const auto idParsed = parseInt64(p.value(idOpt));
    if (!idParsed.has_value()) return fail("--id must be integer");
    const qint64 id = idParsed.value();

    if (!repo.remove(id)) return fail("invoice not found");

    if (isKv(fmt)) print_kv("ok", "1");
    else out << "Deleted invoice id=" << id << "\n";

    return 0;
}

static int dispatchInvoice(const QString& action, const QStringList& cmdArgs,
                           InvoiceService& svc, InvoiceRepo& repo) {
    if (action == "create") return handleInvoiceCreate(cmdArgs, svc);
    if (action == "get")    return handleInvoiceGet(cmdArgs, svc);
    if (action == "list")   return handleInvoiceList(cmdArgs, svc);
    if (action == "edit")   return handleInvoiceEdit(cmdArgs, svc);
    if (action == "delete") return handleInvoiceDelete(cmdArgs, repo);
    return fail("unsupported action for invoice");
}

static int handleInvoiceLineCreate(const QStringList& args, InvoiceLineService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek create invoice-line");
    addCommonOptions(p);

    QCommandLineOption invOpt("invoice", "Invoice id.", "invoice");
    QCommandLineOption descOpt("desc", "Description.", "desc");
    QCommandLineOption qtyOpt("qty", "Quantity.", "qty");
    QCommandLineOption priceOpt("price", "Unit price.", "price");
    QCommandLineOption unitOpt("unit", "Unit (optional).", "unit");
    QCommandLineOption typeOpt("type", "Line type: goods|services (optional).", "type");
    QCommandLineOption lineNoOpt("line-no", "Line number (optional).", "line-no");

    p.addOption(invOpt);
    p.addOption(descOpt);
    p.addOption(qtyOpt);
    p.addOption(priceOpt);
    p.addOption(unitOpt);
    p.addOption(typeOpt);
    p.addOption(lineNoOpt);

    if (!p.parse(args)) return fail(p.errorText());

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();

    if (!p.isSet(invOpt)) return fail("--invoice is required");
    if (!p.isSet(descOpt)) return fail("--desc is required");
    if (!p.isSet(qtyOpt)) return fail("--qty is required");
    if (!p.isSet(priceOpt)) return fail("--price is required");

    const auto invoiceParsed = parseInt64(p.value(invOpt));
    if (!invoiceParsed.has_value()) return fail("--invoice must be integer");
    const qint64 invoice_id = invoiceParsed.value();
    const QString desc = p.value(descOpt);

    const auto qtyParsed = parseDouble(p.value(qtyOpt));
    const auto priceParsed = parseDouble(p.value(priceOpt));
    if (!qtyParsed.has_value()) return fail("--qty must be number");
    if (!priceParsed.has_value()) return fail("--price must be number");
    const double qty = qtyParsed.value();
    const double price = priceParsed.value();

    int line_no = 1;
    if (p.isSet(lineNoOpt)) {
        const auto lineNoParsed = parseInt(p.value(lineNoOpt));
        if (!lineNoParsed.has_value()) return fail("--line-no must be integer");
        line_no = lineNoParsed.value();
    }
    if (line_no <= 0) return fail("--line-no must be positive integer");

    const QString line_type = p.isSet(typeOpt) ? p.value(typeOpt).trimmed() : "goods";
    if (line_type != "goods" && line_type != "services") return fail("--type must be goods|services");

    const QString unit = p.isSet(unitOpt) ? p.value(unitOpt).trimmed() : "pcs";
    if (unit.isEmpty()) return fail("--unit must be non-empty");

    auto r = svc.add(invoice_id, line_no, desc, line_type, qty, unit, price);
    if (!r.ok) return fail(r.error);

    if (isKv(fmt)) print_kv("id", QString::number(r.value));
    else out << "Created invoice-line id=" << r.value << "\n";
    return 0;
}

static int handleInvoiceLineList(const QStringList& args, InvoiceLineService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek list invoice-line");
    addCommonOptions(p);

    QCommandLineOption invOpt("invoice", "Invoice id.", "invoice");
    p.addOption(invOpt);

    if (!p.parse(args)) return fail(p.errorText());
    if (!p.isSet(invOpt)) return fail("--invoice is required");

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    const auto invoiceParsed = parseInt64(p.value(invOpt));
    if (!invoiceParsed.has_value()) return fail("--invoice must be integer");
    const qint64 invoice_id = invoiceParsed.value();

    auto r = svc.list(invoice_id);
    if (!r.ok) return fail(r.error);

    const auto& list = r.value;
    for (const InvoiceLine& l : list) {
        if (isKv(fmt)) {
            printInvoiceLineKv(l);
        } else {
            printInvoiceLineHuman(l);
            out << "\n";
        }
    }
    return 0;
}

static int handleInvoiceLineGet(const QStringList& args, InvoiceLineRepo& repo) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek get invoice-line");
    addCommonOptions(p);

    QCommandLineOption idOpt("id", "Line id.", "id");
    p.addOption(idOpt);

    if (!p.parse(args)) return fail(p.errorText());
    if (!p.isSet(idOpt)) return fail("--id is required");

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    const auto idParsed = parseInt64(p.value(idOpt));
    if (!idParsed.has_value()) return fail("--id must be integer");
    const qint64 id = idParsed.value();

    const auto line = repo.get(id);
    if (!line.has_value()) return fail("invoice_line not found");

    if (isKv(fmt)) printInvoiceLineKv(line.value());
    else printInvoiceLineHuman(line.value());
    return 0;
}

static int handleInvoiceLineDelete(const QStringList& args, InvoiceLineService& svc) {
    QCommandLineParser p;
    p.setApplicationDescription("dopek delete invoice-line");
    addCommonOptions(p);

    QCommandLineOption idOpt("id", "Line id.", "id");
    p.addOption(idOpt);

    if (!p.parse(args)) return fail(p.errorText());
    if (!p.isSet(idOpt)) return fail("--id is required");

    const auto fmtOpt = parseFormat(p);
    if (!fmtOpt.has_value()) return fail("--format must be human|kv");
    const QString fmt = fmtOpt.value();
    const auto idParsed = parseInt64(p.value(idOpt));
    if (!idParsed.has_value()) return fail("--id must be integer");
    const qint64 id = idParsed.value();

    auto r = svc.remove(id);
    if (!r.ok) return fail(r.error);

    if (isKv(fmt)) print_kv("ok", "1");
    else out << "Deleted invoice-line id=" << id << "\n";
    return 0;
}

static int dispatchInvoiceLine(const QString& action, const QStringList& cmdArgs,
                               InvoiceLineService& svc, InvoiceLineRepo& repo) {
    if (action == "create") return handleInvoiceLineCreate(cmdArgs, svc);
    if (action == "get")    return handleInvoiceLineGet(cmdArgs, repo);
    if (action == "list")   return handleInvoiceLineList(cmdArgs, svc);
    if (action == "delete") return handleInvoiceLineDelete(cmdArgs, svc);
    return fail("unsupported action for invoice-line");
}


// -------------------------
// Stubs for other targets
// (to keep main.cpp clean and ready)
// -------------------------

static int dispatchNotImplemented(const QString& target) {
    return fail(QString("target not implemented yet: %1").arg(target));
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("dopek");
    QCoreApplication::setApplicationVersion("0.1");

    // Wiring (in-memory for now)
    CounterpartyRepo counterpartyRepo;
    CounterpartyService counterpartyService(counterpartyRepo);
    InvoiceRepo invoiceRepo;
    InvoiceLineRepo invoiceLineRepo;
    InvoiceService invoiceService(invoiceRepo, counterpartyRepo, invoiceLineRepo);
    InvoiceLineService invoiceLineService(invoiceLineRepo, invoiceRepo, invoiceService);
    // Parse head: action + target
    CommandHead h;
    const QStringList argvList = QCoreApplication::arguments();
    if (!parseHead(argvList, h)) {
        return showTopHelp(QCoreApplication::applicationName());
    }

    // Build command-args for the specific command parser:
    // QCommandLineParser expects argv-like list where [0] is program name.
    // We let each command parser have its own options; no global spaghetti.
    QStringList cmdArgs;
    cmdArgs << argvList[0];        // program name
    cmdArgs << h.tailArgs;         // remaining options only

    // Dispatch by target
    if (h.target == "counterparty" || h.target == "cp") {
        return dispatchCounterparty(h.action, cmdArgs, counterpartyService, counterpartyRepo);
    }

    if (h.target == "invoice" || h.target == "inv") {
        return dispatchInvoice(h.action, cmdArgs, invoiceService, invoiceRepo);
    }
    if (h.target == "invoice-line" || h.target == "line") {
        return dispatchInvoiceLine(h.action, cmdArgs, invoiceLineService, invoiceLineRepo);
    }
    if (h.target == "posting" || h.target == "post") {
        return dispatchNotImplemented("posting");
    }
    if (h.target == "project" || h.target == "prj") {
        return dispatchNotImplemented("project");
    }
    if (h.target == "material" || h.target == "mat") {
        return dispatchNotImplemented("material");
    }
    if (h.target == "inventory" || h.target == "invn") {
        return dispatchNotImplemented("inventory");
    }

    return fail("unknown target");
}
