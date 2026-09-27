from pathlib import Path
import gc
import re

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


RAW_DIR = Path("results/raw")
PLOTS_DIR = Path("results/plots")
PLOTS_DIR.mkdir(parents=True, exist_ok=True)

AMORTIZED_PATTERN = re.compile(
    r"^amortized_(C|D)_i(\d+)_j(\d+)_r(\d+)_(binomial|fibonacci)\.csv$"
)

HEAPS = ["binomial", "fibonacci"]


# ============================================================
# Utilidades generales
# ============================================================

def power_label(value):
    exponent = int(round(np.log2(value)))
    return f"$2^{{{exponent}}}$"


def heap_title(heap_type):
    if heap_type == "binomial":
        return "Binomial"

    if heap_type == "fibonacci":
        return "Fibonacci"

    raise ValueError(
        f"Heap desconocido: {heap_type}"
    )


def save_figure(fig, filename):
    path = PLOTS_DIR / filename

    fig.tight_layout()
    fig.savefig(
        path,
        bbox_inches="tight",
    )

    plt.close(fig)

    print(f"Grafico guardado en {path}")


# ============================================================
# COSTO TOTAL
# ============================================================

def load_total_cost():
    path = RAW_DIR / "total_cost.csv"

    if not path.exists():
        raise FileNotFoundError(
            f"No se encontro {path}"
        )

    data = pd.read_csv(path)

    required_columns = {
        "heap_type",
        "series",
        "vertex_count",
        "edge_count",
        "repetition",
        "mst_weight",
        "total_time_ns",
    }

    missing = (
        required_columns
        - set(data.columns)
    )

    if missing:
        raise ValueError(
            "Faltan columnas en total_cost.csv: "
            + ", ".join(sorted(missing))
        )

    return data


def theoretical_cost(
    heap_type,
    vertex_count,
    edge_count,
):
    v = np.asarray(
        vertex_count,
        dtype=float,
    )

    e = np.asarray(
        edge_count,
        dtype=float,
    )

    if heap_type == "binomial":
        return e * np.log2(v)

    if heap_type == "fibonacci":
        return e + v * np.log2(v)

    raise ValueError(
        f"Heap desconocido: {heap_type}"
    )


def scale_theoretical_curve(
    experimental,
    theoretical,
):
    experimental = np.asarray(
        experimental,
        dtype=float,
    )

    theoretical = np.asarray(
        theoretical,
        dtype=float,
    )

    denominator = np.dot(
        theoretical,
        theoretical,
    )

    if denominator == 0:
        return theoretical

    constant = (
        np.dot(
            experimental,
            theoretical,
        )
        / denominator
    )

    return constant * theoretical


def prepare_total_curve(
    data,
    heap_type,
    series,
):
    filtered = data[
        (data["heap_type"] == heap_type)
        & (data["series"] == series)
    ].copy()

    if filtered.empty:
        raise ValueError(
            f"No hay datos para {heap_type}, serie {series}"
        )

    grouped = (
        filtered
        .groupby(
            [
                "vertex_count",
                "edge_count",
            ],
            as_index=False,
        )
        .agg(
            mean_time_ns=(
                "total_time_ns",
                "mean",
            ),
            std_time_ns=(
                "total_time_ns",
                "std",
            ),
        )
    )

    grouped["std_time_ns"] = (
        grouped["std_time_ns"]
        .fillna(0)
    )

    if series == "A":
        grouped = grouped.sort_values(
            "edge_count"
        )

        x_values = (
            grouped["edge_count"]
            .to_numpy()
        )

        x_name = "Numero de aristas E"

    elif series == "B":
        grouped = grouped.sort_values(
            "vertex_count"
        )

        x_values = (
            grouped["vertex_count"]
            .to_numpy()
        )

        x_name = "Numero de vertices V"

    else:
        raise ValueError(
            f"Serie desconocida: {series}"
        )

    mean_ms = (
        grouped["mean_time_ns"]
        .to_numpy()
        / 1e6
    )

    std_ms = (
        grouped["std_time_ns"]
        .to_numpy()
        / 1e6
    )

    theoretical = theoretical_cost(
        heap_type,
        grouped["vertex_count"],
        grouped["edge_count"],
    )

    theoretical_scaled = (
        scale_theoretical_curve(
            mean_ms,
            theoretical,
        )
    )

    return {
        "x_values": x_values,
        "x_name": x_name,
        "mean": mean_ms,
        "std": std_ms,
        "theoretical": theoretical_scaled,
    }


def plot_total_curve(
    curve,
    heap_type,
    series,
    common_ymax,
):
    positions = np.arange(
        len(curve["x_values"])
    )

    labels = [
        power_label(value)
        for value in curve["x_values"]
    ]

    fig, ax = plt.subplots(
        figsize=(9, 6)
    )

    ax.errorbar(
        positions,
        curve["mean"],
        yerr=curve["std"],
        marker="o",
        linestyle="-",
        capsize=4,
        label="Tiempo experimental promedio",
    )

    if heap_type == "binomial":
        theoretical_label = (
            r"Curva teorica escalada: "
            r"$E\log_2(V)$"
        )
    else:
        theoretical_label = (
            r"Curva teorica escalada: "
            r"$E + V\log_2(V)$"
        )

    ax.plot(
        positions,
        curve["theoretical"],
        linestyle="--",
        label=theoretical_label,
    )

    ax.set_xticks(positions)
    ax.set_xticklabels(labels)

    ax.set_xlabel(
        curve["x_name"]
    )

    ax.set_ylabel(
        "Tiempo total promedio (ms)"
    )

    ax.set_ylim(
        0,
        common_ymax,
    )

    ax.set_title(
        "Costo total - "
        f"Heap {heap_title(heap_type)} "
        f"- Serie {series}"
    )

    ax.grid(
        True,
        linestyle=":",
        alpha=0.5,
    )

    ax.legend()

    save_figure(
        fig,
        f"total_{heap_type}_serie_{series}.pdf",
    )


def generate_total_cost_plots():
    print(
        "\n=== Graficos de costo total ==="
    )

    data = load_total_cost()

    for series in ["A", "B"]:
        curves = {}

        for heap_type in HEAPS:
            curves[heap_type] = (
                prepare_total_curve(
                    data,
                    heap_type,
                    series,
                )
            )

        common_ymax = 0.0

        for heap_type in HEAPS:
            curve = curves[heap_type]

            experimental_max = np.max(
                curve["mean"]
                + curve["std"]
            )

            theoretical_max = np.max(
                curve["theoretical"]
            )

            common_ymax = max(
                common_ymax,
                experimental_max,
                theoretical_max,
            )

        common_ymax *= 1.10

        for heap_type in HEAPS:
            plot_total_curve(
                curves[heap_type],
                heap_type,
                series,
                common_ymax,
            )

    del data
    gc.collect()


# ============================================================
# ARCHIVOS AMORTIZADOS
# ============================================================

def find_amortized_files(
    series,
    heap_type,
):
    configurations = {}

    for path in RAW_DIR.glob(
        f"amortized_{series}_*.csv"
    ):
        match = AMORTIZED_PATTERN.match(
            path.name
        )

        if match is None:
            continue

        file_series = match.group(1)
        i = int(match.group(2))
        j = int(match.group(3))
        repetition = int(
            match.group(4)
        )
        file_heap = match.group(5)

        if file_series != series:
            continue

        if file_heap != heap_type:
            continue

        key = (i, j)

        if key not in configurations:
            configurations[key] = []

        configurations[key].append(
            (repetition, path)
        )

    for key in configurations:
        configurations[key].sort(
            key=lambda item: item[0]
        )

    return configurations


def configuration_label(
    series,
    i,
    j,
):
    if series == "C":
        return (
            f"E={power_label(2 ** j)}"
        )

    if series == "D":
        return (
            f"V={power_label(2 ** i)}"
        )

    raise ValueError(
        f"Serie desconocida: {series}"
    )


# ============================================================
# Lectura amortizada con bajo uso de memoria
# ============================================================

def read_curve_endpoints(
    path,
):
    """
    Primera pasada liviana.

    Solo lee call_count para determinar
    hasta donde llega cada repeticion.
    """
    max_call = None
    min_call = None

    for chunk in pd.read_csv(
        path,
        usecols=["call_count"],
        chunksize=250000,
    ):
        values = (
            chunk["call_count"]
            .to_numpy()
        )

        if len(values) == 0:
            continue

        current_min = values[0]
        current_max = values[-1]

        if min_call is None:
            min_call = current_min
        else:
            min_call = min(
                min_call,
                current_min,
            )

        if max_call is None:
            max_call = current_max
        else:
            max_call = max(
                max_call,
                current_max,
            )

        del chunk

    if min_call is None:
        raise ValueError(
            f"Archivo vacio: {path}"
        )

    return (
        float(min_call),
        float(max_call),
    )


def interpolate_csv_curve(
    path,
    metric,
    common_calls,
):
    """
    Lee un CSV por chunks.

    Solo conserva los puntos necesarios
    para interpolar sobre common_calls.

    Esto evita cargar el CSV completo.
    """
    result = np.empty(
        len(common_calls),
        dtype=float,
    )

    target_index = 0

    previous_call = None
    previous_value = None

    usecols = [
        "call_count",
        metric,
    ]

    for chunk in pd.read_csv(
        path,
        usecols=usecols,
        chunksize=250000,
    ):
        calls = (
            chunk["call_count"]
            .to_numpy(dtype=float)
        )

        values = (
            chunk[metric]
            .to_numpy(dtype=float)
        )

        if len(calls) == 0:
            del chunk
            continue

        for call, value in zip(
            calls,
            values,
        ):
            while (
                target_index
                < len(common_calls)
                and common_calls[
                    target_index
                ] <= call
            ):
                target = common_calls[
                    target_index
                ]

                if (
                    previous_call is None
                    or call == previous_call
                ):
                    interpolated = value
                else:
                    fraction = (
                        (target - previous_call)
                        / (call - previous_call)
                    )

                    interpolated = (
                        previous_value
                        + fraction
                        * (
                            value
                            - previous_value
                        )
                    )

                result[
                    target_index
                ] = interpolated

                target_index += 1

            previous_call = call
            previous_value = value

            if (
                target_index
                >= len(common_calls)
            ):
                break

        del chunk

        if (
            target_index
            >= len(common_calls)
        ):
            break

    if target_index < len(common_calls):
        if previous_value is None:
            raise ValueError(
                f"No se pudo leer {path}"
            )

        result[
            target_index:
        ] = previous_value

    return result


def average_configuration(
    files,
    metric,
):
    """
    Procesa las repeticiones de UNA configuracion.

    Nunca mantiene los CSV completos en memoria.
    """
    if not files:
        raise ValueError(
            "Configuracion sin archivos"
        )

    ranges = []

    for repetition, path in files:
        minimum, maximum = (
            read_curve_endpoints(path)
        )

        ranges.append(
            (minimum, maximum)
        )

    min_common_calls = max(
        minimum
        for minimum, _ in ranges
    )

    max_common_calls = min(
        maximum
        for _, maximum in ranges
    )

    if (
        max_common_calls
        < min_common_calls
    ):
        raise ValueError(
            "Las repeticiones no tienen "
            "un rango comun"
        )

    number_of_points = min(
        300,
        max(
            2,
            int(
                max_common_calls
                - min_common_calls
                + 1
            ),
        ),
    )

    common_calls = np.linspace(
        min_common_calls,
        max_common_calls,
        number_of_points,
    )

    # En vez de guardar las 10 curvas,
    # acumulamos suma y suma de cuadrados.
    total = np.zeros(
        number_of_points,
        dtype=float,
    )

    total_squared = np.zeros(
        number_of_points,
        dtype=float,
    )

    count = 0

    for repetition, path in files:
        print(
            "    leyendo "
            f"{path.name}"
        )

        values = interpolate_csv_curve(
            path,
            metric,
            common_calls,
        )

        total += values
        total_squared += (
            values * values
        )

        count += 1

        del values
        gc.collect()

    mean = total / count

    if count > 1:
        variance = (
            total_squared
            - count * mean * mean
        ) / (count - 1)

        # Evita negativos diminutos por
        # error de punto flotante.
        variance = np.maximum(
            variance,
            0.0,
        )

        std = np.sqrt(
            variance
        )
    else:
        std = np.zeros_like(
            mean
        )

    return (
        common_calls,
        mean,
        std,
    )


# ============================================================
# Preparacion de curvas amortizadas
# ============================================================

def prepare_amortized_heap(
    series,
    heap_type,
    metric,
):
    configurations = (
        find_amortized_files(
            series,
            heap_type,
        )
    )

    if not configurations:
        raise FileNotFoundError(
            "No se encontraron archivos "
            f"para {heap_type}, serie {series}"
        )

    if series == "C":
        ordered_keys = sorted(
            configurations.keys(),
            key=lambda key: key[1],
        )
    else:
        ordered_keys = sorted(
            configurations.keys(),
            key=lambda key: key[0],
        )

    curves = []

    for i, j in ordered_keys:
        files = configurations[
            (i, j)
        ]

        print(
            f"  {heap_type} serie {series}: "
            f"i={i}, j={j}, "
            f"{len(files)} repeticiones"
        )

        (
            calls,
            mean,
            std,
        ) = average_configuration(
            files,
            metric,
        )

        if metric == (
            "cumulative_time_ns"
        ):
            mean = mean / 1e6
            std = std / 1e6

        curves.append(
            {
                "i": i,
                "j": j,
                "calls": calls,
                "mean": mean,
                "std": std,
            }
        )

    return curves


def find_common_amortized_ymax(
    heap_curves,
):
    maximum = 0.0

    for curves in heap_curves.values():
        for curve in curves:
            curve_max = np.max(
                curve["mean"]
                + curve["std"]
            )

            maximum = max(
                maximum,
                curve_max,
            )

    if maximum <= 0:
        maximum = 1.0

    return maximum * 1.10


def plot_amortized_curves(
    curves,
    heap_type,
    series,
    metric,
    common_ymax,
):
    fig, ax = plt.subplots(
        figsize=(9, 6)
    )

    for curve in curves:
        label = configuration_label(
            series,
            curve["i"],
            curve["j"],
        )

        ax.plot(
            curve["calls"],
            curve["mean"],
            label=label,
        )

        ax.fill_between(
            curve["calls"],
            np.maximum(
                0,
                curve["mean"]
                - curve["std"],
            ),
            curve["mean"]
            + curve["std"],
            alpha=0.12,
        )

    ax.set_xlabel(
        "Numero acumulado de llamadas "
        "a decreaseKey"
    )

    ax.set_ylim(
        0,
        common_ymax,
    )

    if metric == (
        "cumulative_time_ns"
    ):
        ax.set_ylabel(
            "Tiempo acumulado promedio (ms)"
        )

        metric_title = (
            "Tiempo acumulado de decreaseKey"
        )

        suffix = "time"

    elif metric == (
        "structural_operations"
    ):
        if heap_type == "binomial":
            ax.set_ylabel(
                "Intercambios acumulados"
            )

            metric_title = (
                "Intercambios acumulados"
            )

        else:
            ax.set_ylabel(
                "Cortes en cascada acumulados"
            )

            metric_title = (
                "Cortes en cascada acumulados"
            )

        suffix = "operations"

    else:
        raise ValueError(
            f"Metrica desconocida: {metric}"
        )

    ax.set_title(
        f"{metric_title} - "
        f"Heap {heap_title(heap_type)} "
        f"- Serie {series}"
    )

    ax.grid(
        True,
        linestyle=":",
        alpha=0.5,
    )

    ax.legend()

    save_figure(
        fig,
        f"amortized_{heap_type}"
        f"_serie_{series}"
        f"_{suffix}.pdf",
    )


# ============================================================
# Generacion amortizada
# ============================================================

def generate_amortized_metric(
    series,
    metric,
):
    print()
    print(
        "Procesando serie "
        f"{series}, metrica {metric}"
    )

    heap_curves = {}

    for heap_type in HEAPS:
        heap_curves[heap_type] = (
            prepare_amortized_heap(
                series,
                heap_type,
                metric,
            )
        )

        gc.collect()

    common_ymax = (
        find_common_amortized_ymax(
            heap_curves
        )
    )

    for heap_type in HEAPS:
        plot_amortized_curves(
            heap_curves[heap_type],
            heap_type,
            series,
            metric,
            common_ymax,
        )

    del heap_curves
    gc.collect()


def generate_amortized_plots():
    print(
        "\n=== Graficos amortizados ==="
    )

    for series in ["C", "D"]:
        generate_amortized_metric(
            series,
            "cumulative_time_ns",
        )

        generate_amortized_metric(
            series,
            "structural_operations",
        )


# ============================================================
# Main
# ============================================================

def main():
    print(
        "=== Generando graficos finales ==="
    )

    generate_total_cost_plots()
    generate_amortized_plots()

    print()
    print(
        "=== Graficos terminados ==="
    )

    print(
        f"Directorio: {PLOTS_DIR}"
    )


if __name__ == "__main__":
    main()